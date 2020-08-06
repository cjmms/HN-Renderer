#include "TriangleHouse.h"



TriangleHouse::TriangleHouse()
{
    float vertices[] = {     0, 0, 0};
    /*
    float vertices[] = {
        -0.5f, -0.5f, 0.5f,  // bottom-left
        0.5f, 0.5f, 0.5f, // top-right
        0.5f, -0.5f, 0.5f, // bottom-right        
        0.5f, 0.5f, 0.5f, // top-right
        -0.5f, -0.5f, 0.5f, // bottom-left
        -0.5f, 0.5f, 0.5f // top-left 
    };
    */
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}



void TriangleHouse::render(glm::mat4 view, glm::mat4 projection, Shader& shader)
{
    shader.setMat4("mvp", projection * view);
    glDrawArrays(GL_POINTS, 0, 6);
}


int runTriangleHouse()
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

    if (glewInit() != GLEW_OK)
        std::cout << "init error" << std::endl;

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);

    Shader shader("res/Shaders/Advanced_OpenGL/TriangleHouse/TriangleHouse.shader");
    //Shader  shader("res/Shaders/Advanced_OpenGL/Build-inVar/PointSize.shader");
    shader.Bind();

    TriangleHouse renderer;

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

        renderer.render(view, projection, shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

