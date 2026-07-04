#pragma once

#include <stdio.h>
#include "GLFW/glfw3.h"

class Window
{
public:
	Window(int width, int height);
	~Window();

	int Initialize();

	GLFWwindow* getGLFWMainWindow() { return gLFWMainWindow; }

	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);

	void CloseWindow();

private:
	int windowWidth, windowHeight;
	GLFWwindow* gLFWMainWindow;

	void PrintHello();

};
