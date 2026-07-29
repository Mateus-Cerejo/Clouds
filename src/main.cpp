#pragma once

#include <stdio.h>
#include <ctime>
#include <thread>
#include <mutex>
#include <iostream>
#include <algorithm>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "Window/Window.h"
#include "InputManager/InputManager.h"
#include "ShaderCreator/ShaderCreator.h"
#include "Shader/Shader.h"
#include "DefaultShader/DefaultShader.h"
#include "Mesh/Mesh.h"
#include "Camera/Camera.h"
#include "Texture/Texture.h"

unsigned int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

Camera camera;

std::vector<Shader*> shaderList;
std::vector<Mesh*> meshList;
std::vector<Texture*> textureList;

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

unsigned int indices[] = {
	0, 1, 3, // front
	3, 1, 2, // front

	3, 4, 0, // top
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


std::atomic<bool> stop_workers = false;
std::atomic<bool> noise_generated = false;

int numRowsColsDepth = 12;
int resolution = 128;


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

float DistToClosest(float x, float y, vector<glm::vec3> points) {
	float minDist = 999999;

	for (size_t i = 0; i < points.size(); i++)
	{
		float dist = glm::length(points[i] - glm::vec3(x, y, 0));

		if (dist < minDist)
		{
			minDist = dist; 
		}
	}

	return minDist;
}

// Generate points in a grid and scatter them with a random offset
void ScatterWorleyNoisePoints(vector<glm::vec3>* points) {
	for (size_t y = 0; y < numRowsColsDepth; y++)
	{
		for (size_t x = 0; x < numRowsColsDepth; x++)
		{
			//for (size_t z = 0; z < numRowsColsDepth; z++)
			//{
				// get a random offset for x,y,z between [0, 0.99]
			glm::vec3 offset(rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f);

			glm::vec3 point(x + offset.x, y + offset.y, /*z + offset.z*/ 0);

			printf("Point: x: %f, y: %f \n", point.x, point.y);

			points->push_back(point);
			//}
		}
	}
}

// For every pixel calculate the distance to the closest point and write it as grayscale color in the bitmap
void CalculateAllPixelColors(char* bitmap, vector<glm::vec3>* points) {
	// Iterate every pixel and write it to the .bmp image in grayscale
	for (float y = 0; y < resolution; y++)
	{
		for (float x = 0; x < resolution; x++)
		{
			float distToClosest = DistToClosest((float)x / resolution * numRowsColsDepth, (float)y / resolution * numRowsColsDepth, *points);

			int value = (int)(distToClosest * 255 / 1.5); // The distance obtained is [0, ~1.4[ so multiply by 255 and divide by 1.5 to get a value between [0, 255[
			value = (value == 10) ? 11 : value; // In windows 10 translates to 0D 0A (carriage return) wich is annoying because every other number (0-255) will be just 1 byte, so yeah here is the fix

			int index = y * (resolution % 4 == 0 ? resolution : resolution + 4 - resolution % 4) * 3 + x * 3;

			// The value is then converted into a grayscale pixel
			bitmap[index] = value; // Blue byte
			bitmap[index + 1] = value; // Green byte
			bitmap[index + 2] = value; // Red byte
		}

		// .bmp files require each line to have a number of pixels divisible my 4, so in case our pixel resolution is not then we add empty data until it is
		for (int i = 0; i < (resolution % 4 == 0 ? 0 : 4 - resolution % 4); i++)
		{
			int index = (y + 1) * (resolution) * 3 + i * 3;
			bitmap[index] = 0;
			bitmap[index + 1] = 0;
			bitmap[index + 2] = 0;
		}
	}
}

// Write headers and bitmap data to .bmp file
void WriteToBMPFile(char* bitmap, string fileName) {
	// Create Bitmap file (.bmp)
	FILE* fp = fopen(fileName.c_str(), "w+");

	char tag[] = { 'B', 'M' };

	int header[] = {
		54 + (resolution + (resolution % 4 == 0 ? 0 : 4 - resolution % 4)) * resolution * 3, // Size of file in bytes
		0, // Reserved stuff
		54, // Byte offset of pixel data
		40, // Size of DIB Header
		resolution, // width
		resolution, // height
		0x180001, // Some stuff
		0, // Compression algo
		0, // Pixel data size in bytes, don't matter if compression 0
		0x0EC3, // not sure but use example
		0x0EC3, // not sure but use example
		0, // Color palette stuff, don't need
		0 // Color palette stuff, don't need
	};

	fwrite(&tag, sizeof(tag), 1, fp);
	fwrite(&header, sizeof(header), 1, fp);
	fwrite(bitmap, (resolution + (resolution % 4 == 0 ? 0 : 4 - resolution % 4)) * resolution * 3 * sizeof(char), 1, fp);
	fclose(fp);
}

// TODO: massivly improve performance by checking only adjacent points for pixels
// TODO: massivly improve performance by doing this in the GPU 
// TODO: add seamless tilling
// TODO: FREE MEMORY
// This will work in a seperate thread because it seems to be quite heavy work
void GenerateWorleyNoise() {
	std::vector<glm::vec3> points;

	// Place points in a grid and offset them randomly
	ScatterWorleyNoisePoints(&points);

	// Array of pixel data for the .bmp image (3 bytes for each pixel, BlueGreenRed)
	char* bitmap = (char*)malloc((resolution + (resolution % 4 == 0 ? 0 : 4 - resolution % 4)) * resolution * 3 * sizeof(char));
	
	// For every pixel calculate distance to closest point and store as color in bitmap
	CalculateAllPixelColors(bitmap, &points);

	// Write the header and bitmap data into the noise.bmp file
	WriteToBMPFile(bitmap, "noise.bmp");

	// notify that the thread work is done
	noise_generated.store(true);
	printf("Noise texture generated\n");
	

	// TODO: Add logic to stop mid work in case of window close
	//while (!stop_workers)
	//{
	//	std::this_thread::sleep_for(std::chrono::seconds(2));
	//}
}

// Setup and use shader and then render meshes
void Render(glm::mat4 projectionMtx, glm::mat4 viewMtx) {
	DefaultShader* defaultShader = dynamic_cast<DefaultShader*>(shaderList[0]);

	// Use shaders and render meshes
	defaultShader->Use();
	int modelMtxLoc = defaultShader->GetModelMatrixLocation();
	int viewMtxLoc = defaultShader->GetViewMatrixLocation();
	glUniformMatrix4fv(viewMtxLoc, 1, GL_FALSE, glm::value_ptr(viewMtx));
	int projMtxLoc = defaultShader->GetProjectionMatrixLocation();
	glUniformMatrix4fv(projMtxLoc, 1, GL_FALSE, glm::value_ptr(projectionMtx));

	int camPosLoc = defaultShader->GetCameraPosLocation();
	glUniform3f(camPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
	
	// Render all clouds
	for (Mesh* cloud : meshList) {

		// Define model matrix to convert to world space
		glm::mat4 model = glm::mat4(1.0f);

		// TODO: Fix problems with transformations on the shader side
		//model = glm::translate(model, glm::vec3(2, 0, -10));
		//model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(2, 4, 9));

		glUniformMatrix4fv(modelMtxLoc, 1, GL_FALSE, glm::value_ptr(model));

		textureList[0]->UseTexture();

		cloud->RenderMesh();
	}
	
	// For tests
	//float offsetX = 2;
	//float offsetY = 2;
	//float offsetZ = 2;
	//for (Mesh* cloud : meshList) {
	//	for (size_t i = 0; i < 50; i++)
	//	{
	//		for (size_t j = 0; j < 50; j++)
	//		{
	//			for (size_t o = 0; o < 50; o++)
	//			{
	//				// Define model matrix to convert to world space
	//				glm::mat4 model = glm::mat4(1.0f);
	//				model = glm::translate(model, glm::vec3(offsetX * i, offsetY * o, offsetZ * j));

	//				glUniformMatrix4fv(modelMtxLoc, 1, GL_FALSE, glm::value_ptr(model));

	//				cloud->RenderMesh();
	//			}
	//		}
	//	}
	//}
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
	shaderList.push_back(&defaultShader);

	// Create and store geometry / metadata of mesh in GPU. VAO, VBOs and EBOs
	Mesh cube;
	cube.CreateMesh(vertices, indices, sizeof(vertices) / sizeof(vertices[0]), sizeof(indices) / sizeof(indices[0]));
	meshList.push_back(&cube);

	Texture noiseTex = Texture("noise2d.bmp");
	noiseTex.LoadTexture(GL_BGR);
	textureList.push_back(&noiseTex);

	// Thread to generate Noise
	std::thread noise_worker(GenerateWorleyNoise);

	// Let OpenGL calculate which elements are in front of what
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE); 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Define projection matrix to clip space with perspective
	glm::mat4 projectionMtx = glm::perspective(glm::radians(70.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 1000.0f);

	// Main loooooop until user closes window
	while (!glfwWindowShouldClose(mainWindow.getGLFWMainWindow()))
	{
		currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// an ok fps counter
		fpsCounter();

		// If noise generated load it once
		if (noise_generated.load())
		{
			// TODO: send texture to GPU
			noise_generated.store(false);
		}

		// Clear screen every frame before drawing
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Render(projectionMtx, camera.CalculateViewMatrix());

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