#pragma once
#include "Essentials.h"
class Surface
{
public:
	VkSurfaceKHR surface = VK_NULL_HANDLE; 
	VkInstance*  instanceRef;	// Check: Add GLFW Surface Create Function
public:
	void Initialize(VkInstance instance,GLFWwindow* window,HWND hwnd,HINSTANCE hInstance)
	{
		instanceRef = &instance;
		

		VkWin32SurfaceCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.pNext = nullptr;
		createInfo.hinstance = hInstance;
		createInfo.hwnd = hwnd;
		
		if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS)
			throw std::runtime_error("Problem!");
		/*
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
			throw std::runtime_error("Problem");
			*/
	}
	
	~Surface()
	{
		vkDestroySurfaceKHR(*instanceRef, surface, nullptr);
	}
	
};
