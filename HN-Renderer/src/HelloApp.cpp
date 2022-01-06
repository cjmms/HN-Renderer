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
        glm::mat4 projection{ 1.f };
        glm::mat4 view{1.f};
        glm::vec4 ambientLightColor{1.f, 1.f, 1.f, 0.02f};
        glm::vec3 lightPos = glm::vec3(-1);
        int tessellationLevel;
        //alignas(16) glm::vec4 lightColor{ 1.0 };
        glm::vec4 lightColor{ 1.0 };
        float height;
        float width;
    };


    HelloTriangleApplication::HelloTriangleApplication() { 
        globalPool = DescriptorPool::Builder(Device)
            .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();

        LoadGameObjs(); 
    }


    HelloTriangleApplication::~HelloTriangleApplication() {}



    void HelloTriangleApplication::run()
    {
        std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);

        // create UBO and map them to the device mem
        for (int i = 0; i < uboBuffers.size(); ++i)
        {
            uboBuffers[i] = std::make_unique<Buffer>(
                Device, sizeof(GlobalUbo),
                1,        
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );

            uboBuffers[i]->map();
        }

        
        // Descriptor Sets layout
        // only 1 buffer for vertex shader
        auto globalSetLayout = DescriptorSetLayout::Builder(Device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
            .build();

        
        // create Descriptor Sets, bind ubo to Descriptor Set
        std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (unsigned int i = 0; i < globalDescriptorSets.size(); ++i)
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            DescriptorWriter(*globalSetLayout, *globalPool).writeBuffer(0, &bufferInfo).build(globalDescriptorSets[i]);
        }
        

        RenderSystem renderSystem{ Device, renderer.GetSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        Camera camera{};


        auto viewer = GameObj::CreateGameObject();
        viewer.transform.translation.z = -10.5f;
        viewer.transform.translation.y = -2.5;
        KeyboardController CameraController{};

        auto currTime = std::chrono::high_resolution_clock::now();

        while (!Window.ShouldClose())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currTime).count();
            currTime = newTime;

            // camera setup
            CameraController.moveInPlane(Window.GetWindow(), frameTime, viewer);
            camera.setViewYXZ(viewer.transform.translation, viewer.transform.rotation);

            float aspect = renderer.GetAspectRatio();
            camera.SetPerspectiveProj(glm::radians(50.0f), aspect, 0.1, 100);
            
            if (auto cmdBuffer = renderer.BeginFrame())
            {
                int frameIndex = renderer.GetFrameIndex();
                // frame info
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    cmdBuffer,
                    camera,
                    globalDescriptorSets[frameIndex]
                };

                //update 
                GlobalUbo ubo{};
                ubo.projection = camera.GetProjMat();       // set ubo data
                ubo.view = camera.GetViewMat();             // set ubo data
                ubo.tessellationLevel = 10;
                ubo.height = 2;
                ubo.width = 0.2;
                uboBuffers[frameIndex]->writeToBuffer(&ubo);   // write to ubo buffer
                uboBuffers[frameIndex]->flush();           // since buffer is not coherent, manually flush


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
        /*
        std::shared_ptr<Model> flatVaseModel = Model::CreateModelFromFile(Device, "Assets/models/flat_vase.obj");

        auto flatVaseObj = GameObj::CreateGameObject();
        flatVaseObj.model = flatVaseModel;
        flatVaseObj.transform.translation = { -0.5, 0.5, 0.f };
        flatVaseObj.transform.scale = glm::vec3(3.0f);

        gameObjs.push_back(std::move(flatVaseObj));

        std::shared_ptr<Model> smoothVaseModel = Model::CreateModelFromFile(Device, "Assets/models/smooth_vase.obj");

        auto smoothVaseObj = GameObj::CreateGameObject();
        smoothVaseObj.model = smoothVaseModel;
        smoothVaseObj.transform.translation = { 0.5, 0.5, 0.f };
        smoothVaseObj.transform.scale = glm::vec3(3.0f);

        gameObjs.push_back(std::move(smoothVaseObj));
        */
        /*
        std::shared_ptr<Model> bunnyModel = Model::CreateModelFromFile(Device, "Assets/models/bunny.obj");

        auto bunnyObj = GameObj::CreateGameObject();
        bunnyObj.model = bunnyModel;
        bunnyObj.transform.translation = { -0.5, 0.5, 0.f };
        bunnyObj.transform.scale = glm::vec3(1, -1, 1);

        gameObjs.push_back(std::move(bunnyObj));
        */

        // floor
        std::shared_ptr<Model> QuadModel = Model::CreateModelFromFile(Device, "Assets/models/Quad.obj");

        auto QuadObj = GameObj::CreateGameObject();
        QuadObj.model = QuadModel;
        QuadObj.transform.translation = { 0.0, 0.5, 0.0f };
        QuadObj.transform.scale = glm::vec3(5.0f, 1.f, 5.0f);

        gameObjs.push_back(std::move(QuadObj));
    }


}