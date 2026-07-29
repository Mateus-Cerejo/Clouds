#include "Texture.h"

Texture::Texture() : Texture((char*)"") {}

Texture::Texture(const char* filePath)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	this->filePath = filePath;
}


bool Texture::LoadTexture(GLint imageFormat)
{
	unsigned char* data = stbi_load(filePath, &width, &height, &bitDepth, 0);
	if (!data)
	{
		printf("Error reading data from file:%s\n", filePath);
		return false;
	}
	
	glGenTextures(1, &textureID); // Generate one texture and set id in textureID
	glBindTexture(GL_TEXTURE_2D, textureID); // Bind it so next commands affect it

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data); // Attach texture data to texture object
	glGenerateMipmap(GL_TEXTURE_2D); // It will only generate one mipmap level because we only added one level

	stbi_image_free(data);

	return false;
}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	filePath = (char*)"";
}

Texture::~Texture()
{
	ClearTexture();
}