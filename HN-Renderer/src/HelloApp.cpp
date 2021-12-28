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


    // temporary helper function, creates a 1x1x1 cube centered at offset
    std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset) 
    {
        std::vector<Model::Vertex> vertices{
            
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},

        };
        for (auto& v : vertices) {
            v.position += offset;
        }
        return std::make_unique<Model>(device, vertices);
    }

    void HelloTriangleApplication::LoadGameObjs()
    {
        std::shared_ptr<Model> model = createCubeModel(Device, {0.0, 0.0, 0.0});

        auto cube = GameObj::CreateGameObject();
        cube.model = model;
        cube.transform.translation = { 0.0, 0.0, 2.5f };
        cube.transform.rotation = { 0.2, 0.5, 0 };
        cube.transform.scale = { 0.5f , 0.5f, 0.5f};

        gameObjs.push_back(std::move(cube));
    }


}