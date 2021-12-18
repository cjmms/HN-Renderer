#pragma once

#include "pch.h"
#include "window.hpp"

namespace HN
{

    class HelloTriangleApplication
    {
    public:
        void run();

    private:
        Window Window{WIDTH, HEIGHT, "hello world"};

    
    };

}