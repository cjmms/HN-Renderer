#pragma once

#include "pch.h"
#include "window.hpp"
#include "GameObj.hpp"
#include "Renderer.hpp"


namespace HN
{

    class HelloTriangleApplication
    {
    public:
        HelloTriangleApplication();
        ~HelloTriangleApplication();
        void run();

        HelloTriangleApplication(const HelloTriangleApplication&) = delete;
        HelloTriangleApplication& operator=(const HelloTriangleApplication&) = delete;

    private:
        void LoadGameObjs();

        Window Window{WIDTH, HEIGHT, "hello world"};
        Device Device{ Window };

        Renderer renderer{ Device, Window };

        std::vector<GameObj> gameObjs;
    };

}