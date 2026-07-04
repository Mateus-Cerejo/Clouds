#pragma once

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>

#include <glad/glad.h>

#include "../Shader/Shader.h"

using namespace std;

class ShaderCreator
{
public:
	ShaderCreator();

	static GLuint CreateFromFile(const char* vertexPath, const char* fragmentPath);

	static void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

	~ShaderCreator();

private:
	static string ReadFile(const char* filePath);

};