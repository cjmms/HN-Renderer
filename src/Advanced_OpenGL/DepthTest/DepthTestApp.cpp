
#include "DepthTestApp.h"



    int run()
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

        glEnable(GL_DEPTH_TEST);

        Shader shader("res/Shaders/Advanced_OpenGL/DepthTest/DepthTest.shader");

        DepthTest renderer;

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

            shader.Bind();
            renderer.renderScene(view, projection, shader);


            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
