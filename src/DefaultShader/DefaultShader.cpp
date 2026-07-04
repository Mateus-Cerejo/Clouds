#include "DefaultShader.h"

DefaultShader::DefaultShader(GLuint shaderID) : Shader(shaderID)
{
}

void DefaultShader::Use()
{
	int vertexColorLocation = glGetUniformLocation(programID, "baseColor");

	glUseProgram(programID);

	glUniform4f(vertexColorLocation, 0.7f, 0.7f, 0.7f, 1.0f);
}

int DefaultShader::GetModelMatrixLocation()
{
	return glGetUniformLocation(programID, "model");
}

int DefaultShader::GetViewMatrixLocation()
{
	return glGetUniformLocation(programID, "view");
}

int DefaultShader::GetProjectionMatrixLocation()
{
	return glGetUniformLocation(programID, "projection");
}

int DefaultShader::GetCameraPosLocation()
{
	return glGetUniformLocation(programID, "cameraPos");
}
