#pragma once

#include "pch.h"
#include "window.hpp"
#include "Pipeline.hpp"

namespace HN
{

    class HelloTriangleApplication
    {
    public:
        void run();

    private:
        Window Window{WIDTH, HEIGHT, "hello world"};
        Device Device{ Window };
        Pipeline Pipeline{
            Device, 
            "src/Shaders/simple_shader.vert.spv", 
            "src/Shaders/simple_shader.frag.spv",
            Pipeline::DefaultPipelineConfigInfo(WIDTH, HEIGHT) };
    
    };

}