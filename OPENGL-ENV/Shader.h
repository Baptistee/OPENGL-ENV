#pragma once

#include <string>
#include <GL/glew.h>

class Shader
{
public:
	Shader(const std::string& fileName);
	~Shader();

	void Bind();

	GLuint getProgramID();


private:
	static const unsigned int NUM_SHADERS = 2;

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
};

