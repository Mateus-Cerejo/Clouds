#pragma once

#include <glad/glad.h>

class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateMesh(float* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);

	//TODO create class Object that has a mesh and position rotation and stuff
	//void MoveObject();

	void RenderMesh();

	void ClearMesh();

private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int indexCount;

	//glm::vec3 pos;
	//glm::vec3 rotation;
};