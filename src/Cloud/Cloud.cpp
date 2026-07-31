#include "Cloud.h"

Cloud::Cloud() : Cloud(nullptr, 0){}

Cloud::Cloud(float* verticies, int numOfVertices)
{
	this->vertices = verticies;
	unsigned int* indices = nullptr; // Not used rn
	int numOfIndices = 0; // Not used rn
	mesh.CreateMesh(vertices, indices, numOfVertices, numOfIndices);
}

void Cloud::Render()
{
	mesh.RenderMesh();
}

Cloud::~Cloud()
{
	mesh.ClearMesh();
}