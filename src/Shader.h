#pragma once
#include <string>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};


class Shader 
{
private:
	std::string shaderFilePath;

	// program ID of shader
	unsigned int rendererID;

public:
    Shader(const std::string& path);
	~Shader();
	void Bind();
	void unBind();
	
private:
	// path: res/Shaders/basic.shader
	// path is relative a path
	// absolute path is ($SolutionDir)path
	ShaderProgramSource parseShader(const std::string& path);

	// type: GL_FRAGMENT_SHADER or GL_VERTEX_SHADER
	// return shader program ID
	unsigned int compileShader(unsigned int type, const std::string& shaderSource);
	void shaderErrorInfo(unsigned int shader, unsigned int type);

	// vertexShader: shader source as a string
	// fragmentShader: shader source as a string
	// return shader ID
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
};