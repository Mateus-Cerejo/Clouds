#pragma once

#include <stdio.h>
#include <ctime>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window/Window.h"
#include "InputManager/InputManager.h"
#include "ShaderCreator/ShaderCreator.h"
#include "Shader/Shader.h"
#include "DefaultShader/DefaultShader.h"
#include "Mesh/Mesh.h"
#include "Camera/Camera.h"

unsigned int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

std::vector<Shader> shaderList;

Camera camera;

float vertices[] = {
	 0.5f,  0.5f, 0.5f,  // front top right
	 0.5f, -0.5f, 0.5f,  // front bottom right
	-0.5f, -0.5f, 0.5f,  // front bottom left
	-0.5f, 0.5f, 0.5f,    // front top left

	0.5f,  0.5f, -0.5f,  // back top right
	 0.5f, -0.5f, -0.5f,  // back bottom right
	-0.5f, -0.5f, -0.5f,  // back bottom left
	-0.5f, 0.5f, -0.5f    // back top left
};

unsigned int indices[] = {
	0, 1, 3, // front
	3, 2, 1, // front

	3, 0, 4, // top
	3, 7, 4, // top

	4, 5, 7, // back
	7, 6, 5, // back

	2, 1, 5, // bottom
	2, 5, 6, // bottom

	3, 2, 6, // left
	6, 7, 3, // left

	0, 1, 5, // right
	5, 4, 0  // right
};

float lastFrame = 0.0f;
float currentFrame = 0.0f;
float deltaTime = 0.0f;

float lastFPSFrame = 0.0f;
int fps = 0;


void fpsCounter() {
	if (currentFrame - lastFPSFrame > 1)
	{
		printf("fps: %d\n", fps / (int)(currentFrame - lastFPSFrame));
		lastFPSFrame = currentFrame;
		fps = 0;
	}
	else
	{
		fps++;
	}
}

std::atomic<bool> stop_workers = false;

int numRowsColsDepth = 2;
int resolution =  64;

//struct Point
//{
//	glm::vec3 pos
//};

float DistToClosest(int x, int y, vector<glm::vec3> points) {
	float dist = 999999;

	for (size_t i = 0; i < points.size(); i++)
	{
		if (abs(points[i].x - x) + abs(points[i].y - y) < dist)
		{
			dist = abs(points[i].x - x) + abs(points[i].y - y);
		}
	}

	return dist;
}

// This will work in a seperate thread because it seems to be quite heavy work
void GeneratePerlinNoise() {

	std::vector<glm::vec3> points;

	// scatter points in a grid
	for (size_t y = 0; y < numRowsColsDepth; y++)
	{
		for (size_t x = 0; x < numRowsColsDepth; x++)
		{
			//for (size_t z = 0; z < numRowsColsDepth; z++)
			//{
				// get a random offset for x,y,z between [0, 0.99]
				glm::vec3 offset(rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f);

				glm::vec3 point(x + offset.x, y + offset.y, /*z + offset.z*/ 0);

				points.push_back(point);
			//}
		}
	}

	//string line;
	//ifstream myfile("Untitled.bmp");
	//if (myfile.is_open())
	//{
	//	while (getline(myfile, line))
	//	{
	//		cout << line << '\n';
	//	}
	//	myfile.close();
	//}

	//else cout << "Unable to open file";

	//ofstream myfile("asdf.txt");
	//if (myfile.is_open())
	//{
	//	myfile << "This is a line.\n";
	//	myfile << "This is another line.\n";
	//	myfile.close();
	//}
	//else cout << "Unable to open file";

	// define texture rez
	// for each pixel measure distance to closest point in grid and write to file


	// for now just one 2d image
	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			float distToClosest = DistToClosest(x, y, points);

			printf("dist to closest for x: %d, y: %d, is: %f\n", x, y, distToClosest);
		}
	}


	// notify that the thread work is done

	// just for example
	//while (!stop_workers)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
	printf("Noise texture generated\n");
}

int main()
{
	// For fps calculation
	lastFrame = (float)glfwGetTime();

	// Create window
	Window mainWindow = Window(WINDOW_WIDTH, WINDOW_HEIGHT);
	mainWindow.Initialize();

	// Initialize Glad to use OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	camera = Camera(glm::vec3(-1.0f, 1.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -70.0f, -20.0f, 10.0f, 0.06f);

	// Create shaders
	DefaultShader defaultShader = DefaultShader(ShaderCreator::CreateFromFile("assets/shaders/default.vert", "assets/shaders/default.frag"));

	// Create and store geometry / metadata of mesh in GPU. VAO, VBOs and EBOs
	Mesh cube = Mesh();
	cube.CreateMesh(vertices, indices, sizeof(vertices) / sizeof(vertices[0]), sizeof(indices) / sizeof(indices[0]));
	//Mesh top = Mesh();
	//top.CreateMesh(vertices, tops, sizeof(vertices) / sizeof(vertices[0]), sizeof(tops) / sizeof(tops[0]));

	// Thread to generate Noise
	std::thread noise_worker(GeneratePerlinNoise);

	// Let OpenGL calculate which elements are in front of what
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Main loooooop until user closes window
	while (!glfwWindowShouldClose(mainWindow.getGLFWMainWindow()))
	{
		currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// an ok fps counter
		fpsCounter();

		// Clear screen every frame before drawing
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Define model matrix to convert to world space
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		//model = glm::rotate(model, (float)glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, (float)glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// Define view matrix to convert to view space
		glm::mat4 view;
		//view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
		//	glm::vec3(0.0f, 0.0f, 0.0f),
		//	glm::vec3(0.0f, 1.0f, 0.0f));
		view = camera.CalculateViewMatrix();

		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		//view = glm::rotate(view, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		// Define projection matrix to clip space with perspective
		glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 1000.0f);

		// Use shaders and render meshes
		defaultShader.Use();
		int modelMtxLoc = defaultShader.GetModelMatrixLocation();
		glUniformMatrix4fv(modelMtxLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewMtxLoc = defaultShader.GetViewMatrixLocation();
		glUniformMatrix4fv(viewMtxLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projMtxLoc = defaultShader.GetProjectionMatrixLocation();
		glUniformMatrix4fv(projMtxLoc, 1, GL_FALSE, glm::value_ptr(proj));

		int camPosLoc = defaultShader.GetCameraPosLocation();
		glUniform3f(camPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		cube.RenderMesh();

		// Swaps the front and back color buffer to show what was drawn. 
		// This is done because if there was only one buffer the screen would 
		// flicker as it would be rendering and showing at the same time
		glfwSwapBuffers(mainWindow.getGLFWMainWindow());

		// Take care of events like get keys and mouse and stuff, if not, nothing happens
		glfwPollEvents();
		camera.HandleMovement(deltaTime);
		camera.HandleRotation();
	}

	// since the thread could be writting to a file, tell it to wrap it up and then wait for it to finish
	stop_workers = true;
	noise_worker.join();

	return 0;
}