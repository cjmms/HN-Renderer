#include "ComputeShaderDemo.h"

ComputeShaderDemo::ComputeShaderDemo()
{
    


    //Shader cs("res/Shaders/ComputeShaderDemo/generateTexture.shader");
    //cs.Bind();
    

    // dimensions of the image
    int tex_w = 512, tex_h = 512;
    
    glGenTextures(1, &tex_output);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_output);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tex_w, tex_h, 0, GL_RGBA, GL_FLOAT,
        NULL);
    glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);


    int work_grp_cnt[3];

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);

    printf("max global (total) work group counts x:%i y:%i z:%i\n",
        work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);

    
    /*
    const char* name = "res/Shaders/ComputeShaderDemo/generateTexture.shader";
    GLuint ray_shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(ray_shader, 1, &name, NULL);
    glCompileShader(ray_shader);
    // check for compilation errors as per normal here

    ray_program = glCreateProgram();
    glAttachShader(ray_program, ray_shader);
    glLinkProgram(ray_program);
    */

    
    
    // Create the compute program, to which the compute shader will be assigned

    int gComputeProgram = glCreateProgram();

    std::ifstream input{ "res/Shaders/ComputeShaderDemo/generateTexture.shader" };
    std::string source{ std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>() };

    // Create and compile the compute shader

    GLuint mComputeShader = glCreateShader(GL_COMPUTE_SHADER);

    const char* src = source.c_str();

    glShaderSource(mComputeShader, 1, &src, NULL);

    glCompileShader(mComputeShader);

    // Check if there were any issues when compiling the shader

    int rvalue;

    glGetShaderiv(mComputeShader, GL_COMPILE_STATUS, &rvalue);

    if (!rvalue) {
        GLchar log[10240];
        GLsizei length;
        glGetShaderInfoLog(mComputeShader, 10239, NULL, log);
        std::cout << "ERROR: Compute Shader Compilation Error." << log << std::endl;
    }

    // Attach and link the shader against to the compute program

    glAttachShader(gComputeProgram, mComputeShader);

    glLinkProgram(gComputeProgram);

    // Check if there were some issues when linking the shader.

    glGetProgramiv(gComputeProgram, GL_LINK_STATUS, &rvalue);

    if (!rvalue)

    {
        GLchar log[10240];
        GLsizei length;
        glGetShaderInfoLog(mComputeShader, 10239, NULL, log);
        std::cout << "ERROR: Compute Shader Linking Error." << log << std::endl;
    }


    {
        glUseProgram(gComputeProgram);
        glDispatchCompute((GLuint)512, (GLuint)512, 1);
    }

    // make sure writing to image has finished before read
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT); 
    





    float planeVertices[] = {
        // positions            // normals         // texcoords
         0.5f, 0.5f,  -1.0f,  0.0f, 1.0f, 0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f,  -1.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -0.5f, 0.5f,  -1.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

         0.5f, 0.5f,  -1.0f,  0.0f, 1.0f, 0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f,  -1.0f,  0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f
    };



    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);




}



void ComputeShaderDemo::render(glm::mat4 view, glm::mat4 projection, Shader& shader, Texture& texture)
{
    

    shader.Bind();
    glm::mat4 model(1.0);
    model = glm::translate(model, glm::vec3(0.0, 1.0, 0.0));
    shader.setMat4("mvp", projection * view * model);

    //texture.bindTexture(GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_output);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}



int runComputeShaderDemo()
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

    Shader shader("res/Shaders/ComputeShaderDemo/basic.shader");
    shader.Bind();

    Texture texture("res/Textures/wood.jpg", JPG);
    shader.setInt("texture_wood", 0);


    ComputeShaderDemo renderer;

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

        renderer.render(view, projection, shader, texture);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}