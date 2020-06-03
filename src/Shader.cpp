#include "Shader.h"



Shader::Shader(const std::string& path)
	:rendererID(0), shaderFilePath(path)
{
	ShaderProgramSource source = parseShader(shaderFilePath);
    rendererID = createShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() 
{
	glDeleteProgram(rendererID);
}



ShaderProgramSource Shader::parseShader(const std::string& path) 
{
    enum class ShaderMode {
        NONE = -1, VERTEX_MODE = 0, FRAGMENT_MODE = 1
    };
    std::fstream stream(path);
    std::string line;

    // 2 string streams, one for vertex shader, one for fragment shader
    std::stringstream ss[2];
    ShaderMode currentMode = ShaderMode::NONE;

    // shader file has 2 lines: #shader fragment and #shader vertex 
    // if any or these lines are detected, switch mode, output to another stringstream
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


unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    unsigned int shaderProgram = glCreateProgram();
    unsigned int vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(shaderProgram, vertexShaderID);
    glAttachShader(shaderProgram, fragmentShaderID);

    // TODO: catch exceptions
    glLinkProgram(shaderProgram);

    //TODO: catch exceptions
    glValidateProgram(shaderProgram);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return shaderProgram;
}

unsigned int Shader::compileShader(unsigned int type, const std::string& shaderSource) 
{
    unsigned int id = glCreateShader(type);
    const char* source = shaderSource.c_str();

    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);
    shaderErrorInfo(id, type);
    return id;
}

void Shader::shaderErrorInfo(unsigned int shader, unsigned int type) 
{
    GLint success;
    char infoLog[512];
    // success is a flag, it will be set to False if things went wrong
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR: " 
            << (type == GL_VERTEX_SHADER ? "Vertex " : "Fragment ")
            << "Shader Compilation Error. " << infoLog << std::endl;
    }
}

void Shader::Bind() 
{
    glUseProgram(rendererID);
}

void Shader::unBind() 
{
    glUseProgram(0);
}


int Shader::getUniformLocation(const char* name)
{
    return glGetUniformLocation(rendererID, name);
}

unsigned int Shader::getRendererID() 
{
    return rendererID;
}


void Shader::setMat4(const char* name, glm::mat4 matrix)
{
    unsigned int location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}


void Shader::setVec3(const char* name, glm::vec3 vec)
{
    unsigned int location = getUniformLocation(name);
    glUniform3fv(location, 1, glm::value_ptr(vec));
}