
#include "ParticleSimulation.h"



ParticleSystem* ParticleSimulator::CreateParticleEffect(glm::vec2 dir, float mag, int num)
{
	glm::vec2 pos(gen_random(0.0, 300.0), gen_random(0.0, 300.0));


	return new ParticleSystem();

}


/*
glm::vec2 ParticleSimulator::GenRandomCircSectorDir(float angle1, float angle2)
{
	float angle = gen_random(angle1, angle2);	// random angle from 0 to 2 PI
	return glm::vec2(cosf(angle), sinf(angle));
}


glm::vec2 ParticleSimulator::GenRandomCircDir()
{
	return GenRandomCircSectorDir(0.0f, 2.0f * 3.1415f);
}

*/



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

    // spawn in square, move in circular direction

    // number of particles, size of particles
    ParticleConfig pCon(1000, 160);     
    // spawn center, spawn area size, spawn area shape
    SpawnConfig sCon(glm::vec2(500.0f), 100.0f, AREA_MODE::SQUARE);   
    // move direction, speed, move pattern
    MoveConfig mCon(glm::vec2(0.0, 1.0), 1.7f, DIR_MODE::CIRCULAR);
    ParticleSystem particleSystem(sCon, mCon, pCon);

    // spawn in circle, move in sector dir
    //SpawnConfig sCon(glm::vec2(500.0f), 10.0f, AREA_MODE::CIRCLE);
    //MoveConfig mCon(glm::vec2(0.00001f, 0.0), 3.1415f * 0.3f, 0.7f, DIR_MODE::CIRCULAR_SECTOR);
    //ParticleSystem particleSystem(sCon, mCon, 10000);
    
    //ParticleSystem particleSystem(1000000);

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