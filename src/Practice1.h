#pragma once
#include "Shader.h"


class Practice1 {

private:
    GLuint VBO, VAO, EBO;
    Shader shader;

public:
    Practice1();
    void render();

};
