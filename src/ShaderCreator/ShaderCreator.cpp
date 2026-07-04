#include "ShaderCreator.h"

GLuint ShaderCreator::CreateFromFile(const char* vertexPath, const char* fragmentPath)
{
	string vertexString = ReadFile(vertexPath);
	string fragmentString = ReadFile(fragmentPath);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	GLuint shaderProgram = glCreateProgram();

	if (!shaderProgram)
	{
		printf("Error creating shader program\n");
		return NULL;
	}

	AddShader(shaderProgram, vertexCode, GL_VERTEX_SHADER);
	AddShader(shaderProgram, fragmentCode, GL_FRAGMENT_SHADER);

	glLinkProgram(shaderProgram);
	
	return shaderProgram;
}


string ShaderCreator::ReadFile(const char* filePath)
{
	string content;
	ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open())
	{
		printf("Failed to read %s! File doesn't exist", filePath);
		return "";
	}

	string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();

	return content;
}

void ShaderCreator::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = (GLint)strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0; 
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}
