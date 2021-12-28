#include "pch.h"
#include "HelloApp.h"
#include "RenderSystem.hpp"
#include "Camera.hpp"
#include "KeyboardController.hpp"

namespace HN
{
    HelloTriangleApplication::HelloTriangleApplication()
    {
        LoadGameObjs();
    }



    HelloTriangleApplication::~HelloTriangleApplication()
    {
        
    }



    void HelloTriangleApplication::run()
    {
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
                renderer.BeginSwapChainRenderPass(cmdBuffer);

                renderSystem.RenderGameObjs(cmdBuffer, gameObjs, camera);

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