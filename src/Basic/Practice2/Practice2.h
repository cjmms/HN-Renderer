// This Practice draws a Triangle
// Color of this triangle is passed by an additional vertex attribute

#pragma once
#include "../../Shader.h"

class Practice2
{
private:
    GLuint VBO, VAO;

public:
    Practice2();
    void render();
};

