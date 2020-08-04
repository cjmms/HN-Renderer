#include "Build_inVar.h"

Build_inVar::Build_inVar()
{
    cubeInit();
}


void Build_inVar::cubeInit()
{
    float vertices[] = {
        // back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right    
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right              
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left                
        // front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right        
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left        
        // left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left       
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        // right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right      
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right          
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
        // bottom face          
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left        
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        // top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right                 
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left  
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top-left              
    };

    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}


void Build_inVar::drawPoints(glm::mat4 mvp, Shader& shader)
{
    shader.Bind();
    shader.setMat4("mvp", mvp);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_POINTS, 0, 36);
}


void Build_inVar::drawTriangle(glm::mat4 mvp, Shader& shader)
{
    shader.Bind();
    shader.setMat4("mvp", mvp);

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}


void Build_inVar::render(glm::mat4 view, glm::mat4 projection, Shader& shader)
{
    drawPoints(projection * view, shader);
}


static unsigned int num = 1;
static unsigned int maxNum = 3;    // maximum practices

void static key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_N && action == GLFW_PRESS)
    {
        num++;
        if (num > maxNum) num = 1;
    }
}



int runBuild_inVar() 
{

        GLFWwindow* window;

        // Initialize the library 
        if (!glfwInit())
            return -1;

        // Create a windowed mode window and its OpenGL context 
        window = glfwCreateWindow(1200, 1000, "Vanilla", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // Make the window's context current 
        glfwMakeContextCurrent(window);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        glfwSetKeyCallback(window, key_callback);

        if (glewInit() != GLEW_OK)
            std::cout << "init error" << std::endl;

        glEnable(GL_DEPTH_TEST);


        Shader pointSizeShader("res/Shaders/Advanced_OpenGL/Build-inVar/PointSize.shader");
        Shader colorCubeShader("res/Shaders/Advanced_OpenGL/Build-inVar/ColorCube.shader");
        Shader frontFacingShader("res/Shaders/Advanced_OpenGL/Build-inVar/FrontFacing.shader");

        glEnable(GL_PROGRAM_POINT_SIZE);


        Build_inVar renderer;

        // Loop until the user closes the window 
        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            // Render here 
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            camera.cameraUpdateFrameTime();

            glm::mat4 projection = camera.getProjectionMatrix();
            glm::mat4 view = camera.getViewMatrix();


            switch (num) {
            case 1:
                //std::cout << 1 << std::endl;
                renderer.drawPoints(projection * view, pointSizeShader);
                break;
            case 2:
                renderer.drawTriangle(projection * view, colorCubeShader);
                break;
            case 3:
                renderer.drawTriangle(projection * view, frontFacingShader);
                break;
            default:
                std::cout << "num error" << std::endl;
            }


            //renderer.render(view, projection, shader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    
}