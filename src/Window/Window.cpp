#include "Window.h"
#include "../InputManager/InputManager.h"
#include "stb_image.h"

Window::Window(int width, int height) {
	windowWidth = width;
	windowHeight = height;
	gLFWMainWindow = NULL;
}

void Window::CloseWindow() {
	glfwSetWindowShouldClose(gLFWMainWindow, true);
}

int Window::Initialize() {
	PrintHello();

	if (!glfwInit())
	{
		printf("Yeah just give up buddy (glfwInit failed)\n");
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); for MacOS

	gLFWMainWindow = glfwCreateWindow(windowWidth, windowHeight, "Clouds - If you are looking for realistic clouds look outside", NULL, NULL);
	
	GLFWimage images[1];
	images[0].pixels = stbi_load("assets/icons/icon.png", &images[0].width, &images[0].height, 0, 4);

	glfwSetWindowIcon(gLFWMainWindow, 1, images);

	if (gLFWMainWindow == NULL)
	{
		printf("GLFW window creation failed!!!\n");
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(gLFWMainWindow);

	//glViewport(0, 0, windowWidth, windowHeight);

	glfwSetFramebufferSizeCallback(gLFWMainWindow, FrameBufferSizeCallback);

    glfwSetKeyCallback(gLFWMainWindow, HandleKeys);
	glfwSetCursorPosCallback(gLFWMainWindow, HandleMouse);

	InputManager::GetInstance()->RegisterKeyFunction(GLFW_KEY_ESCAPE, [=] {
		CloseWindow();
	});

	// Lock cursor
	glfwSetInputMode(gLFWMainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return 0;
}

void Window::PrintHello() {
	printf("There are no clouds but soon there will\n");
}

void Window::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		InputManager::GetInstance()->KeyPressed(key);
	}
	else if (action == GLFW_RELEASE)
	{
		InputManager::GetInstance()->KeyReleased(key);
	}
}

void Window::HandleMouse(GLFWwindow* window, double xPos, double yPos)
{
	InputManager::GetInstance()->MouseMoved(xPos, yPos);
}

Window::~Window() {
	glfwDestroyWindow(gLFWMainWindow);
	glfwTerminate();
}
