#include "WindowHandler.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
void WindowHandler::InitializeWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	mainWindow = glfwCreateWindow(windowWidth, windowHeight, "Mad Renderer", nullptr, nullptr);

	vulkanRenderer->Start(mainWindow);
}

void WindowHandler::MainLoop()
{
	while (!glfwWindowShouldClose(mainWindow))
	{
		glfwPollEvents();
		vulkanRenderer->Update();
	}

	CleanUp();
}

void WindowHandler::CleanUp()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}

WindowHandler::WindowHandler(int windowWidth, int windowHeight)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
}

