#pragma once

#include "stb_image.h"
#include <glad/glad.h>

class Texture
{
public:
	Texture();
	Texture(const char* filePath);
	~Texture();

	bool LoadTexture(GLint imageFormat);
	void UseTexture();
	void ClearTexture();

private:
	unsigned int textureID;
	int width, height, bitDepth;

	const char* filePath;
};