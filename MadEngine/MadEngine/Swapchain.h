#pragma once
#include "BufferHandler.h"

class SwapChain
{
public:
	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapchainImageViews;
public:
	std::vector<VkImage> swapChainImages;
	VkSurfaceFormatKHR surfaceFormat;
private:
	VkDevice* deviceRef;
	VkImage _computeImage;
	VkImageView computeImageView;
	VkDeviceMemory computeDeviceMemory;
private:

	VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& avaibleFormats)
	{
		for (const auto& avaibleFormat : avaibleFormats)
		{
			if (avaibleFormat.format == VK_FORMAT_B8G8R8A8_SRGB && avaibleFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return avaibleFormat;
			}
		}

		return avaibleFormats[0];
	}

	VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& avaiblePresentModes)
	{
		for (const auto& avaiblePresentMode : avaiblePresentModes)
		{
			if (avaiblePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return avaiblePresentMode;
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
			return capabilities.currentExtent;
		else
		{
			VkExtent2D actualExtent = { 1800,1000 }; // Check: Add Resolution Settings
			actualExtent.width = Max(capabilities.minImageExtent.width, Min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = Max(capabilities.minImageExtent.height, Min(capabilities.maxImageExtent.width, actualExtent.height));

			return actualExtent;
		}


	}
public:
	void CreateComputeImages(uint32_t width, uint32_t height,VkDevice logicalDevice,Buffer* bufferHandler,VkPhysicalDevice physicalDevice)
	{
		VkImageCreateInfo computeImage = {};
		computeImage.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		computeImage.imageType = VK_IMAGE_TYPE_2D;
		computeImage.format = surfaceFormat.format;
		computeImage.extent = { width,height,1 };
		computeImage.mipLevels = 1;
		computeImage.arrayLayers = 1;
		computeImage.samples = VK_SAMPLE_COUNT_1_BIT;
		computeImage.tiling = VK_IMAGE_TILING_OPTIMAL;
		computeImage.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		computeImage.usage = VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		VkResult result = vkCreateImage(logicalDevice, &computeImage, nullptr, &_computeImage);
		if (result != VK_SUCCESS)
			throw std::runtime_error("Problem");

		uint32_t memoryTypeIndex = 0;
		bufferHandler->FindMemoryType(memoryTypeIndex, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, physicalDevice, logicalDevice);

		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(logicalDevice, _computeImage, &memoryRequirements);

		VkMemoryAllocateInfo memoryInfo = {};
		memoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryInfo.allocationSize = memoryRequirements.size;
		memoryInfo.memoryTypeIndex = memoryTypeIndex;

		result = vkAllocateMemory(logicalDevice, &memoryInfo, nullptr, &computeDeviceMemory);
		if (result != VK_SUCCESS)
			throw std::runtime_error("Problem");
		result = vkBindImageMemory(logicalDevice, _computeImage, computeDeviceMemory, 0);
		if (result != VK_SUCCESS)
			throw std::runtime_error("Problem");

		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = _computeImage;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = surfaceFormat.format;
		imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		imageViewCreateInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT,0,1,0,1 };

		result = vkCreateImageView(logicalDevice, &imageViewCreateInfo, nullptr, &computeImageView);
		if (result != VK_SUCCESS)
			throw std::runtime_error("Problem!");
	}

	void CreateImageViews(VkDevice device)
	{
		swapchainImageViews.resize(swapChainImages.size());

		for (size_t i = 0; i < swapChainImages.size(); i++)
		{
			VkImageViewCreateInfo imageViewCreateInfo = {};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.image = swapChainImages[i];
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; // Check
			imageViewCreateInfo.format = swapChainImageFormat;
			imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(device, &imageViewCreateInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS)
				throw std::runtime_error("Problem!");
		}
	}

public:

	uint32_t GetImagesSize()
	{
		return static_cast<uint32_t>(swapChainImages.size());
	}
	void Initialize(VkDevice device, VkSurfaceKHR surface, SwapchainSupportDetails swapChainSupportDetails, uint32_t graphicsFamilyQueueIndex, uint32_t presentFamilyQueueIndex)
	{
		deviceRef = &device;
		SwapchainSupportDetails swapChainSupport = swapChainSupportDetails;

	    surfaceFormat = ChooseSwapchainSurfaceFormat(swapChainSupport.surfaceFormats);
		VkPresentModeKHR presentMode = ChooseSwapchainPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = surface;
		swapchainCreateInfo.minImageCount = imageCount;
		swapchainCreateInfo.imageFormat = surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = extent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		if (graphicsFamilyQueueIndex != presentFamilyQueueIndex)
		{
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainCreateInfo.queueFamilyIndexCount = 2;
			uint32_t queueFamilyIndices[] = { presentFamilyQueueIndex,graphicsFamilyQueueIndex };
			swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapchainCreateInfo.queueFamilyIndexCount = 0;
			swapchainCreateInfo.pQueueFamilyIndices = nullptr;
		}
		swapchainCreateInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = presentMode;
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapChain) != VK_SUCCESS)
			throw std::runtime_error("Problem!");

		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;

		CreateImageViews(device);
	}

	~SwapChain()
	{
		for (auto imageView : swapchainImageViews)
			vkDestroyImageView(*deviceRef, imageView, nullptr);
		vkDestroySwapchainKHR(*deviceRef, swapChain, nullptr);
	}
};


