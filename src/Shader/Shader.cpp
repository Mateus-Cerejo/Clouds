#include "Shader.h"

Shader::Shader(GLuint shaderID) {
	this->programID = shaderID;
	uniformPointLightPosition = glGetUniformLocation(shaderID, "pl_position");
}

void Shader::Use()
{
}

void Shader::UseShader() {
	glUseProgram(programID);
}

void Shader::ClearShader()
{
	if (programID != 0)
	{
		glDeleteProgram(programID);
		programID = 0;
	}
}

Shader::~Shader() {
	ClearShader();
}
