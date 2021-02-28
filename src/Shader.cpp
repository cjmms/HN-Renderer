#include "Shader.h"



Shader::Shader(const std::string& path)
	:rendererID(0), shaderFilePath(path)
{
	ShaderProgramSource source = parseShader(shaderFilePath);
    rendererID = createShader(  source.VertexSource, 
                                source.FragmentSource, 
                                source.GeometrySource,
                                source.ComputeSource);
}

Shader::~Shader() 
{
	glDeleteProgram(rendererID);
}



ShaderProgramSource Shader::parseShader(const std::string& path) 
{
    enum class ShaderMode {
        NONE = -1, VERTEX_MODE = 0, FRAGMENT_MODE = 1, GEOMETRY_MODE = 2, COMPUTE_MODE = 3
    };
    std::fstream stream(path);
    std::string line;

    // 3 string streams, one for vertex shader, one for fragment shader, one for geometry shader
    std::stringstream ss[4];
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
            else if (  line.find("geometry") != std::string::npos && 
                      !(line.find("//") != std::string::npos))
                currentMode = ShaderMode::GEOMETRY_MODE;
            else if (line.find("compute") != std::string::npos) {
                currentMode = ShaderMode::COMPUTE_MODE;
            }
        }
        else
        {
            ss[(int)currentMode] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str(), ss[2].str(), ss[3].str() };
}


unsigned int Shader::createShader(  const std::string& vertexShader, 
                                    const std::string& fragmentShader,
                                    const std::string& geometryShader,
                                    const std::string& computeShader) 
{
    bool hasVertexShader = vertexShader.length() > 0;
    bool hasFragmentShader = fragmentShader.length() > 0;
    bool hasGeometryShader = geometryShader.length() > 0;
    bool hasComputeShader = computeShader.length() > 0;


    unsigned int shaderProgram = glCreateProgram();
    unsigned int vertexShaderID;
    unsigned int fragmentShaderID;
    unsigned int geometryShaderID;
    unsigned int computeShaderID;

    if (hasVertexShader)
    {
        vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexShader);
        glAttachShader(shaderProgram, vertexShaderID);
    }

    if (hasFragmentShader)
    {
        fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
        glAttachShader(shaderProgram, fragmentShaderID);
    }


    if (hasGeometryShader) {
        geometryShaderID = compileShader(GL_GEOMETRY_SHADER, geometryShader);
        glAttachShader(shaderProgram, geometryShaderID);
    }

    if (hasComputeShader) {
        computeShaderID = compileShader(GL_COMPUTE_SHADER, computeShader);
        glAttachShader(shaderProgram, computeShaderID);
    }

    if (hasComputeShader) {
        int rvalue;
        glGetShaderiv(computeShaderID, GL_COMPILE_STATUS, &rvalue);
        fprintf(stderr, "Error in compiling the compute shader\n");
        GLchar log[10240];
        GLsizei length;
        glGetShaderInfoLog(computeShaderID, 10239, NULL, log);
        std::cout << "ERROR: Compute Shader Compilation Error." << log << std::endl;
        exit(40);
    }

    // TODO: catch exceptions
    glLinkProgram(shaderProgram);

    //TODO: catch exceptions
    glValidateProgram(shaderProgram);

    if (hasVertexShader) glDeleteShader(vertexShaderID);
    if (hasFragmentShader) glDeleteShader(fragmentShaderID);
    if (hasGeometryShader) glDeleteShader(geometryShaderID);
    if (hasComputeShader) glDeleteShader(computeShaderID);

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
        if (type == GL_VERTEX_SHADER)
            std::cout << "ERROR: Vertex Shader Compilation Error. " << infoLog << std::endl;
        else if (type == GL_FRAGMENT_SHADER)
            std::cout << "ERROR: Fragment Shader Compilation Error. " << infoLog << std::endl;
        else if (type == GL_GEOMETRY_SHADER)
            std::cout << "ERROR: Geometry Shader Compilation Error. " << infoLog << std::endl;
        else
            std::cout << "ERROR: Compute Shader Compilation Error." << infoLog << std::endl;
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


void Shader::setMat4(const std::string &name, glm::mat4 matrix)
{
    setMat4(name.c_str(), matrix);
}



void Shader::setVec3(const char* name, glm::vec3 vec)
{
    unsigned int location = getUniformLocation(name);
    glUniform3fv(location, 1, glm::value_ptr(vec));
}


void Shader::setVec3(const std::string& name, glm::vec3 vec)
{
    setVec3(name.c_str(), vec);
}



void Shader::setVec2(const char* name, glm::vec2 vec)
{
    unsigned int location = getUniformLocation(name);
    glUniform2fv(location, 1, glm::value_ptr(vec));
}


void Shader::setVec2(const std::string &name, glm::vec2 vec)
{
    setVec2(name.c_str(), vec);
}



void Shader::setFloat(const char* name, float value)
{
    unsigned int location = getUniformLocation(name);
    glUniform1f(location, value);
}



void Shader::setFloat(const std::string& name, float value)
{
    setFloat(name.c_str(), value);
}



void Shader::setInt(const char* name, int value) 
{
    unsigned int location = getUniformLocation(name);
    glUniform1i(location, value);
}



void Shader::setInt(const std::string& name, int value)
{
    setInt(name.c_str(), value);
}