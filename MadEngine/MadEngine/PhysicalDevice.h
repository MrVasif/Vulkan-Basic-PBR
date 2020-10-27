#pragma once
#include "Essentials.h"
struct QueueFamilyIndex
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	bool IsCompleted()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class PhysicalDevice
{
public:
	VkSurfaceKHR surfaceRef;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	uint32_t graphicsQueueFamily = 0;
	uint32_t presentQueueFamily = 0;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	const std::vector<const char*> deviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
public:
	float pushConstant = 1.0f;
private:
	VkSampleCountFlagBits GetMaxSampleCount()
	{
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice,&physicalDeviceProperties);

		VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

		if (counts & VK_SAMPLE_COUNT_64_BIT)
			return VK_SAMPLE_COUNT_64_BIT;
		else if (counts & VK_SAMPLE_COUNT_32_BIT)
			return VK_SAMPLE_COUNT_32_BIT;
		else if (counts & VK_SAMPLE_COUNT_16_BIT)
			return VK_SAMPLE_COUNT_16_BIT;
		else if (counts & VK_SAMPLE_COUNT_8_BIT)
			return VK_SAMPLE_COUNT_8_BIT;
		else if (counts & VK_SAMPLE_COUNT_4_BIT)
			return VK_SAMPLE_COUNT_4_BIT;
		else if (counts & VK_SAMPLE_COUNT_2_BIT)
			return VK_SAMPLE_COUNT_2_BIT;
		else
			return VK_SAMPLE_COUNT_1_BIT;
	}
	SwapchainSupportDetails QuerySupport(VkPhysicalDevice device)
	{
		SwapchainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surfaceRef, &details.capabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surfaceRef, &formatCount, nullptr);
		details.surfaceFormats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surfaceRef, &formatCount, details.surfaceFormats.data());

		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surfaceRef, &presentModeCount, nullptr);
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surfaceRef, &presentModeCount, details.presentModes.data());

		return details;
	}

	bool CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> avaibleExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, avaibleExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& avaibleExtension : avaibleExtensions)
			requiredExtensions.erase(avaibleExtension.extensionName);

		return requiredExtensions.empty();
	}

	bool CheckDeviceAviability(VkPhysicalDevice device)
	{
		QueueFamilyIndex index = FindQueueFamilies(device);

		bool supportExtensions = CheckDeviceExtensionSupport(device);

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);


		bool swapChainSupport = false;
		if (supportExtensions)
		{
			SwapchainSupportDetails swapChainDetails = QuerySupport(device);
			swapChainSupport = !swapChainDetails.surfaceFormats.empty() && !swapChainDetails.presentModes.empty();
		}

		return index.IsCompleted() && supportExtensions && swapChainSupport && supportedFeatures.samplerAnisotropy &&  supportedFeatures.multiViewport;

	}

	QueueFamilyIndex FindQueueFamilies(VkPhysicalDevice device)
	{
		QueueFamilyIndex indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
				graphicsQueueFamily = i;
			}
			VkBool32 supportPresentKHR = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surfaceRef, &supportPresentKHR);

			std::cout << supportPresentKHR << std::endl;
			if (supportPresentKHR)
			{
				indices.presentFamily = i;
				presentQueueFamily = i;
			}

			if (indices.IsCompleted()) {
				break;
			}

			i++;
		}

		return indices;
	}

public:

	VkPhysicalDeviceProperties GetPhysicalDeviceProperties()
	{
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
		assert(sizeof(pushConstant) <= deviceProperties.limits.maxPushConstantsSize);
		return deviceProperties;
	}
	void Initialize(VkInstance instance, VkSurfaceKHR* surface)
	{
		surfaceRef = *surface;

		uint32_t physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

		std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
		vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.data());

		for (const auto& device : physicalDevices)
		{
			if (CheckDeviceAviability(device))
			{
				physicalDevice = device;
				msaaSamples = GetMaxSampleCount();
				break;
			}
		}

		if (physicalDevice == VK_NULL_HANDLE)
			throw std::runtime_error("Problem! ");
	}

	SwapchainSupportDetails GetSwapChainSupportDetails()
	{
		return QuerySupport(physicalDevice);
	}
};