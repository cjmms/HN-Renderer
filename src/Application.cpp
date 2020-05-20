#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

struct ShaderProgramSource 
{
    std::string VertexSource;
    std::string FragmentSource;
};

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void shaderErrorInfo(GLuint shader);
std::string shaderParser(const std::string& path);
ShaderProgramSource parseShader(const std::string& path);

GLuint VBO, VAO;
GLuint vertexShader, fragmentShader, shaderProgram;

// triangle
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "init error" << std::endl;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    ShaderProgramSource shaderSource = parseShader("res/Shaders/basic.shader");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = shaderSource.VertexSource.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    shaderErrorInfo(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = shaderSource.FragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    shaderErrorInfo(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

 

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

GLuint compileShader();

void shaderErrorInfo(GLuint shader) 
{
    GLint success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR: Shader Compilation Error. " << infoLog << std::endl;
    }
}



ShaderProgramSource parseShader(const std::string& path)
{
    enum class ShaderMode {
        NONE = -1, VERTEX_MODE = 0, FRAGMENT_MODE = 1
    };
    std::fstream stream(path);
    std::string line;

    std::stringstream ss[2];
    ShaderMode currentMode = ShaderMode::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                currentMode = ShaderMode::VERTEX_MODE;
            else if (line.find("fragment") != std::string::npos)
                currentMode = ShaderMode::FRAGMENT_MODE;
        }
        else
        {
            ss[(int)currentMode] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}