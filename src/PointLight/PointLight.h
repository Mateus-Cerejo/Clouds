#pragma once

#include <GLM/glm.hpp>
#include "../Mesh/Mesh.h"

class PointLight
{
public:
	PointLight();
	PointLight(glm::vec3 position, float intensity, glm::vec3 color, Mesh mesh);

	void UseLight(float intensityLoc, float colorLoc, float positionLoc);

	~PointLight();

private:
	glm::vec3 position;
	
	float intensity;
	glm::vec3 color;

	Mesh mesh;
};