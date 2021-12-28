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
        std::shared_ptr<Model> model = Model::CreateModelFromFile(Device, "Assets/models/flat_vase.obj");

        auto obj = GameObj::CreateGameObject();
        obj.model = model;
        obj.transform.translation = { 0.0, 0.5, 2.5f };
        //obj.transform.rotation = { 0.2, 0.5, 0 };
        obj.transform.scale = glm::vec3(3.0f);

        gameObjs.push_back(std::move(obj));
    }


}