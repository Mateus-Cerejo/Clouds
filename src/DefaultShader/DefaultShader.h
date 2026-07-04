#pragma once

#include <cstdio>

#include "../Shader/Shader.h"

class DefaultShader : public Shader
{
public:
	DefaultShader(GLuint shaderID);
	//~DefaultShader(); //TODO WHAT SHOULD I DO WITH THIS??? ITS NOT WORK LIKE CONSTRUCTOR EXTENSION
	void Use() override;

	int GetModelMatrixLocation();
	int GetViewMatrixLocation();
	int GetProjectionMatrixLocation();

	int GetCameraPosLocation();

private:

};