#pragma once
#include <glfw3.h>
#include "VulkanRenderer.h"
class WindowHandler
{
private:
	VulkanRenderer* vulkanRenderer = new VulkanRenderer;
public:
	GLFWwindow* mainWindow;

	int windowWidth = 0;

	int windowHeight = 0;

public:
	WindowHandler(int windowWidth, int windowHeight);

	void InitializeWindow();

	void MainLoop();

	void KeyCallback();

private:
	void CleanUp();
};