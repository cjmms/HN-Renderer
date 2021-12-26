#include "pch.h"
#include "HelloApp.h"
#include "RenderSystem.hpp"

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

        while (!Window.ShouldClose())
        {
            glfwPollEvents();
            
            if (auto cmdBuffer = renderer.BeginFrame())
            {
                renderer.BeginSwapChainRenderPass(cmdBuffer);

                renderSystem.RenderGameObjs(cmdBuffer, gameObjs);

                renderer.EndSwapChainRenderPass(cmdBuffer);
                renderer.EndFrame();
            }
        }
    }




    void HelloTriangleApplication::LoadGameObjs()
    {
        // model data
        std::vector<Model::Vertex> vertices{
            {{0.0f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}}
        };

        auto model = std::make_shared<Model>(Device, vertices);

        // transform data
        Transform2dComponent transform{};
        transform.translation = {0.2f, 0.0f};
        transform.scale = { 2.0, 0.6 };

        // put all data together
        auto triangle = GameObj::CreateGameObject();
        triangle.model = model;
        triangle.color = {0.1f, 0.8f, 0.1f};
        triangle.transform2d = transform;
        

        gameObjs.push_back(std::move(triangle));      
    }


}