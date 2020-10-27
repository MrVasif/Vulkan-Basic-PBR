#pragma once
#include "Essentials.h"
class LogicalDevice
{
public:
	VkDevice logicalDevice;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	void Initialize(VkPhysicalDevice physicalDevice, std::vector<const char*> deviceExtensions, uint32_t graphicsQueueIndex, uint32_t presentQueueIndex, float priority = 1.0f)
	{
		std::vector<VkDeviceQueueCreateInfo> uniqueQueueFamilies;

		VkDeviceQueueCreateInfo graphicsQueueCreateInfo = {};
		graphicsQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		graphicsQueueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
		graphicsQueueCreateInfo.queueCount = 1;
		graphicsQueueCreateInfo.pQueuePriorities = &priority;
		uniqueQueueFamilies.push_back(graphicsQueueCreateInfo);

		VkDeviceQueueCreateInfo presentQueueCreateInfo = {};
		presentQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		presentQueueCreateInfo.queueFamilyIndex = presentQueueIndex;
		presentQueueCreateInfo.queueCount = 1;
		presentQueueCreateInfo.pQueuePriorities = &priority;
		uniqueQueueFamilies.push_back(presentQueueCreateInfo);

		VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
		physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;

		VkDeviceCreateInfo deviceCreateInfo = {};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(uniqueQueueFamilies.size());
		deviceCreateInfo.pQueueCreateInfos = uniqueQueueFamilies.data();
		deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
		deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		deviceCreateInfo.enabledLayerCount = 0;
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
		deviceCreateInfo.ppEnabledLayerNames = nullptr;

		if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &logicalDevice) != VK_SUCCESS)
			throw std::runtime_error("Problem!");

		vkGetDeviceQueue(logicalDevice, graphicsQueueIndex, 0, &graphicsQueue);
		vkGetDeviceQueue(logicalDevice, presentQueueIndex, 0, &presentQueue);
	}
	~LogicalDevice()
	{
		vkDestroyDevice(logicalDevice, nullptr);
	}
};
