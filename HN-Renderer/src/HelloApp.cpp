#include "pch.h"
#include "HelloApp.h"
#include "RenderSystem.hpp"
#include "Camera.hpp"
#include "KeyboardController.hpp"
#include "Buffer.hpp"

namespace HN
{
    // uniform buffer object
    struct GlobalUbo 
    {
        glm::mat4 projectionView{ 1.f };
        glm::vec3 lightDir = glm::normalize(glm::vec3(1, -3, -1));
    };


    HelloTriangleApplication::HelloTriangleApplication() { LoadGameObjs(); }


    HelloTriangleApplication::~HelloTriangleApplication() {}



    void HelloTriangleApplication::run()
    {
        // create UBO buffer
        Buffer uboBuffer{
            Device, sizeof(GlobalUbo),
            SwapChain::MAX_FRAMES_IN_FLIGHT,        // UBO count depends on number of frames, so that no need to worry about synch problem
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,    // no coherent because we want selective flush
            Device.properties.limits.minUniformBufferOffsetAlignment
        }; 

        uboBuffer.map();

        RenderSystem renderSystem{ Device, renderer.GetSwapChainRenderPass() };
        Camera camera{};
        //camera.SetOrthProj(-1, 1, -1, 1, -1, 1);

        auto viewer = GameObj::CreateGameObject();
        KeyboardController CameraController{};

        auto currTime = std::chrono::high_resolution_clock::now();

        while (!Window.ShouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currTime).count();
            currTime = newTime;

            CameraController.moveInPlane(Window.GetWindow(), frameTime, viewer);
            camera.setViewYXZ(viewer.transform.translation, viewer.transform.rotation);

            float aspect = renderer.GetAspectRatio();
            camera.SetPerspectiveProj(glm::radians(50.0f), aspect, 0.1, 10);
            
            if (auto cmdBuffer = renderer.BeginFrame())
            {
                int frameIndex = renderer.GetFrameIndex();
                // frame info
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    cmdBuffer,
                    camera
                };

                //update 
                GlobalUbo ubo{};
                ubo.projectionView = camera.GetProjMat() * camera.GetViewMat(); // set ubo data
                uboBuffer.writeToIndex(&ubo, frameIndex);   // write to ubo buffer with frame index
                uboBuffer.flushIndex(frameIndex);           // since buffer is not coherent, manually flush


                // render
                renderer.BeginSwapChainRenderPass(cmdBuffer);

                renderSystem.RenderGameObjs(frameInfo, gameObjs);

                renderer.EndSwapChainRenderPass(cmdBuffer);
                renderer.EndFrame();
            }
        }
    }



    void HelloTriangleApplication::LoadGameObjs()
    {
        std::shared_ptr<Model> flatVaseModel = Model::CreateModelFromFile(Device, "Assets/models/flat_vase.obj");

        auto flatVaseObj = GameObj::CreateGameObject();
        flatVaseObj.model = flatVaseModel;
        flatVaseObj.transform.translation = { -0.5, 0.5, 2.5f };
        flatVaseObj.transform.scale = glm::vec3(3.0f);

        gameObjs.push_back(std::move(flatVaseObj));

        std::shared_ptr<Model> smoothVaseModel = Model::CreateModelFromFile(Device, "Assets/models/smooth_vase.obj");

        auto smoothVaseObj = GameObj::CreateGameObject();
        smoothVaseObj.model = smoothVaseModel;
        smoothVaseObj.transform.translation = { 0.5, 0.5, 2.5f };
        smoothVaseObj.transform.scale = glm::vec3(3.0f);

        gameObjs.push_back(std::move(smoothVaseObj));
    }


}