#include "PointLight.h"

PointLight::PointLight()
{

}

PointLight::PointLight(glm::vec3 position, float intensity, glm::vec3 color, Mesh mesh)
{
	this->position = position;
	this->intensity = intensity;
	this->color = color;
	this->mesh = mesh;
}

void PointLight::UseLight(float intensityLoc, float colorLoc, float positionLoc)
{
	glUniform1f(intensityLoc, intensity);
	glUniform3f(colorLoc, color.x, color.y, color.z);
	glUniform3f(positionLoc, position.x, position.y, position.z);
}

PointLight::~PointLight()
{
}
