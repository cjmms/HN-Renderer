
#include "ParticleSimulation.h"



ParticleSystem* ParticleSimulator::CreateParticleEffect(glm::vec2 dir, float mag, int num)
{
	glm::vec2 pos(gen_random(0.0, 300.0), gen_random(0.0, 300.0));


	return new ParticleSystem();

}



glm::vec2 ParticleSimulator::GenRandomCircSectorDir(float angle1, float angle2)
{
	float angle = gen_random(angle1, angle2);	// random angle from 0 to 2 PI
	return glm::vec2(cosf(angle), sinf(angle));
}


glm::vec2 ParticleSimulator::GenRandomCircDir()
{
	return GenRandomCircSectorDir(0.0f, 2.0f * 3.1415f);
}





int runPariticleSimulation()
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

    ParticleSimulator simulator;
    glm::vec2 dir = simulator.GenRandomCircDir();
   // ParticleSystem *particleSystem1 = simulator.CreateParticleEffect(dir, 0.6f, 10000);


    SpawnConfig sCon(glm::vec2(500.0f), 200.0f, CIRCLE);

    ParticleSystem particleSystem(sCon, glm::vec2(0.0, 0.6), 10000);
    

    particleSystem.Init();
    particleSystem.Print();

    // Loop until the user closes the window 
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Render here 
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        particleSystem.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    particleSystem.Destory();

    glfwTerminate();
    return 0;
}