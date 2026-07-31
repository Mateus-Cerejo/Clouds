#pragma once

#include "../Mesh/Mesh.h"

class Cloud
{
public:
	Cloud();
	Cloud(float* verticies, int numOfVertices);
	~Cloud();

	void Render();

private:
	Mesh mesh;
	float* vertices;
};