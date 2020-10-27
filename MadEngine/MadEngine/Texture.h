#pragma once
#include "BufferHandler.h"
#include <cstring>
#include <stb_image.h>
class Texture
{
public:	
	VkImageView textureImageView;
	VkImageView textureImageView2;
	VkImageView textureImageViewNormal;
	VkImageView textureImageViewMetallic;
	VkImageView textureImageViewRoughness;
	VkImageView textureImageViewAO;
	VkImageView irradianceCubeImageView;
	VkImage offscreenImageView;

	VkSampler textureSampler;
	VkSampler textureSampler2;
	VkSampler textureSamplerNormal;
	VkSampler textureSamplerMetallic;
	VkSampler textureSamplerRoughness;
	VkSampler textureSamplerAO;
	VkSampler irradianceCubeSampler;

	VkImage textureImage;
	VkImage textureImage2;
	VkImage textureImageNormal;
	VkImage textureImageMetallic;
	VkImage textureImageRoughness;
	VkImage textureImageAO;
	VkImage irradianceCubeImage;
	VkImage offscreenImage;

	VkDeviceMemory textureImageMemory;
	VkDeviceMemory textureImageMemory2;
	VkDeviceMemory textureImageMemoryNormal;
	VkDeviceMemory textureImageMemoryMetallic;
	VkDeviceMemory textureImageMemoryRoughness;
	VkDeviceMemory textureImageMemoryAO;
	VkDeviceMemory irradianceCubeDeviceMemory;
	VkDeviceMemory offscreenDeviceMemory;

	VkImageView depthImageView;
	VkImageView colorImageView;

	uint32_t cubeTextureMipMapLevel;
private:
	VkDevice* deviceRef;
	VkImage depthImage;
	VkImage colorImage;
	VkDeviceMemory depthImageMemory;
	VkDeviceMemory colorImageMemory;
private:
	VkFormat FindSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties properties;
			vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &properties);

			if (tiling == VK_IMAGE_TILING_LINEAR && (properties.linearTilingFeatures & features) == features)
				return format;
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (properties.optimalTilingFeatures & features) == features)
				return format;
		}

		throw std::runtime_error("Problem!");
	}
public:

	VkFormat FindDepthFormat(VkPhysicalDevice physicalDevice)
	{
		return FindSupportedFormat(physicalDevice,
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	}

	void InitializeColorResources(VkPhysicalDevice physicalDevice,VkDevice device,Buffer* bufferHandler,VkExtent2D extent,
		VkSampleCountFlagBits sampleCount)
	{
		CreateImage(physicalDevice, device, bufferHandler, extent.width,extent.height, VK_FORMAT_R8G8B8A8_SRGB,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory, false, sampleCount);
	
		colorImageView = CreateImageView(device, colorImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, colorImageView, false);
	}

	void InitializeDepthBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkExtent2D extent, Buffer* buffer, VkSampleCountFlagBits sampleCount)
	{
		VkFormat depthFormat = FindDepthFormat(physicalDevice);

		CreateImage(physicalDevice, device, buffer, extent.width, extent.height, depthFormat, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			depthImage, depthImageMemory,false, sampleCount);
		depthImageView = CreateImageView(device, depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, depthImageView,false);
	}

	void InitializeCubeMapTexture(VkDevice device, VkQueue graphicsQueue, VkCommandPool& commandPool,
		VkPhysicalDevice physicalDevice, Buffer* bufferHandler, VkImage& textureImage, VkDeviceMemory& textureImageMemory, std::string texturePath1, std::string texturePath2, std::string texturePath3,
		std::string texturePath4, std::string texturePath5, std::string texturePath6)
	{
		/*
		deviceRef = &device;
		int textureWidth = 0, textureHeight = 0, textureChannels = 0;
		stbi_uc* pixels1 = stbi_load(texturePath1.c_str(), &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha);
		stbi_uc* pixels2 = stbi_load(texturePath2.c_str(), &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha);
		stbi_uc* pixels3 = stbi_load(texturePath3.c_str(), &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha);
		stbi_uc* pixels4 = stbi_load(texturePath4.c_str(), &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha);
		stbi_uc* pixels5 = stbi_load(texturePath5.c_str(), &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha);
		stbi_uc* pixels6 = stbi_load(texturePath6.c_str(), &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha);
		std::vector<stbi_uc*> pixels = { pixels1,pixels2,pixels3,pixels4,pixels5,pixels6 };
		VkDeviceSize textureSize = textureWidth * textureHeight * 4 * 6;
		VkDeviceSize layerSize = textureSize / 6;
		if (!pixels1 || !pixels2 || !pixels3 || !pixels4 || !pixels5 || !pixels6)
			throw std::runtime_error("Problem!");

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		bufferHandler->CreateBuffer(textureSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device, physicalDevice);

		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, textureSize, 0, &data);
		for(int i = 0; i < 6; i++)
			memcpy(data, pixels[i], static_cast<size_t>(layerSize));
		vkUnmapMemory(device, stagingBufferMemory);

		stbi_image_free(pixels1);
		stbi_image_free(pixels2);
		stbi_image_free(pixels3);
		stbi_image_free(pixels4);
		stbi_image_free(pixels5);
		stbi_image_free(pixels6);

		CreateImage(physicalDevice, device, bufferHandler, textureWidth, textureHeight, VK_FORMAT_R8G8B8A8_SRGB,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory,true, VK_SAMPLE_COUNT_1_BIT);

		bufferHandler->TransitionImageLayout(device, graphicsQueue, commandPool, textureImage,
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,true);

		bufferHandler->CopyBufferToImage(device, commandPool, graphicsQueue, stagingBuffer,
			textureImage, static_cast<uint32_t>(textureWidth), static_cast<uint32_t>(textureHeight),true);

		bufferHandler->TransitionImageLayout(device, graphicsQueue, commandPool, textureImage,
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,true);

		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);
		*/
	}

	void InitializeIrradianceCubeImage(VkPhysicalDevice physicalDevice, VkDevice device, Buffer* bufferHandler)
	{
		const VkFormat format = VK_FORMAT_R32G32B32A32_SFLOAT;
		const int32_t dimension = 64;
		const uint32_t numMips = static_cast<uint32_t>(floor(log2(dimension))) + 1;

		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.extent.width = dimension;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.height = dimension;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = numMips;
		imageInfo.arrayLayers = 6;
		imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		imageInfo.format = format;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateImage(device, &imageInfo, nullptr, &irradianceCubeImage) != VK_SUCCESS)
			throw std::runtime_error("Problem!");

		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(device, irradianceCubeImage, &memoryRequirements);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memoryRequirements.size;
		allocateInfo.memoryTypeIndex = bufferHandler->FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, physicalDevice, device);

		if (vkAllocateMemory(device, &allocateInfo, nullptr, &irradianceCubeDeviceMemory) != VK_SUCCESS)
			throw std::runtime_error("Problem!");

		vkBindImageMemory(device, irradianceCubeImage, irradianceCubeDeviceMemory, 0);


		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = irradianceCubeImage;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
		viewInfo.subresourceRange.layerCount = 6;
		viewInfo.subresourceRange.levelCount = numMips;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.baseArrayLayer = 0;


		if (vkCreateImageView(device, &viewInfo, nullptr, &irradianceCubeImageView) != VK_SUCCESS)
			throw std::runtime_error("Problem!");


		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.maxAnisotropy = 1.0f;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = static_cast<float>(numMips);
		samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;


		if (vkCreateSampler(device, &samplerInfo, nullptr, &irradianceCubeSampler) != VK_SUCCESS)
			throw std::runtime_error("Problem!");


		VkImageCreateInfo offScreenImageInfo = {};
		offScreenImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		offScreenImageInfo.imageType = VK_IMAGE_TYPE_2D;
		offScreenImageInfo.format = format;
		offScreenImageInfo.extent.width = dimension;
		offScreenImageInfo.extent.height = dimension;
		offScreenImageInfo.extent.depth = 1;
		offScreenImageInfo.mipLevels = 1;
		offScreenImageInfo.arrayLayers = 1;
		offScreenImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		offScreenImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		offScreenImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		offScreenImageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		offScreenImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(device, &offScreenImageInfo, nullptr, &offscreenImage) != VK_SUCCESS)
			throw std::runtime_error("Problem!");

		VkMemoryRequirements memoryRequirements2;
		vkGetImageMemoryRequirements(device, offscreenImage, &memoryRequirements2);

		VkMemoryAllocateInfo allocateInfo2 = {};
		allocateInfo2.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo2.allocationSize = memoryRequirements2.size;
		allocateInfo2.memoryTypeIndex = bufferHandler->FindMemoryType(memoryRequirements2.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, physicalDevice, device);

		if (vkAllocateMemory(device, &allocateInfo2, nullptr, &offscreenDeviceMemory) != VK_SUCCESS)
			throw std::runtime_error("Problem!");

		vkBindImageMemory(device, offscreenImage, offscreenDeviceMemory, 0);

		VkImageViewCreateInfo offScreenViewInfo = {};
		offScreenViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		offScreenViewInfo.image = offscreenImage;
		offScreenViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		offScreenViewInfo.subresourceRange.layerCount = 1;
		offScreenViewInfo.subresourceRange.levelCount = 1;
		offScreenViewInfo.format = format;
		offScreenViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		offScreenViewInfo.subresourceRange.baseMipLevel = 0;
		offScreenViewInfo.subresourceRange.baseArrayLayer = 0;


		if (vkCreateImageView(device, &offScreenViewInfo, nullptr, &offscreenImageView) != VK_SUCCESS)
			throw std::runtime_error("Problem!");
	}



	void CreateImage(VkPhysicalDevice physicalDevice, VkDevice device, Buffer* bufferHandler, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, bool isCubeMapImage, VkSampleCountFlagBits sampleCount, uint32_t mipmapLevel = 1)
	{
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.depth = 1;
		if (!isCubeMapImage)
		{
			imageInfo.arrayLayers = 1;
			imageInfo.mipLevels = 1;			
			imageInfo.extent.height = height;
			imageInfo.extent.width = width;
			imageInfo.usage = usage;
		}
		else
		{
			//imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			imageInfo.extent.width = width;
			imageInfo.extent.height = height;
			imageInfo.usage = usage;
			//imageInfo.mipLevels = mipmapLevel;
			imageInfo.arrayLayers = 6;
			imageInfo.mipLevels = 1;
			imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		}
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.samples = sampleCount;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		
		if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
			throw std::runtime_error("Problem!");

		VkMemoryRequirements memoryRequirements;
		vkGetImageMemoryRequirements(device, image, &memoryRequirements);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = memoryRequirements.size;
		allocateInfo.memoryTypeIndex = bufferHandler->FindMemoryType(memoryRequirements.memoryTypeBits, properties, physicalDevice, device);

		if (vkAllocateMemory(device, &allocateInfo, nullptr, &imageMemory) != VK_SUCCESS)
			throw std::runtime_error("Problem!");

		vkBindImageMemory(device, image, imageMemory, 0);
	}

	VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView& textureImageView,bool isCubeMap,uint32_t mipmapLevel = 1)
	{
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		if (isCubeMap)
		{
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
			viewInfo.subresourceRange.layerCount = 6;
			//viewInfo.subresourceRange.levelCount = mipmapLevel;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseMipLevel = 0;
		}
		else
		{
			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.subresourceRange.layerCount = 1;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
		}
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;


		VkImageView imageView;
		if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
			throw std::runtime_error("Problem!");

		return imageView;
	}
	void InitializeTextures(VkDevice device, VkQueue graphicsQueue, VkCommandPool& commandPool,
		VkPhysicalDevice physicalDevice, Buffer* bufferHandler, std::string texturePath, VkImage& textureImage, VkDeviceMemory& textureImageMemory,bool isCubeMap = false)
	{
		deviceRef = &device;
		int textureWidth = 0, textureHeight = 0, textureChannels = 0;
		stbi_uc* pixels = stbi_load(texturePath.c_str(), &textureWidth, &textureHeight, &textureChannels, STBI_rgb_alpha);
		VkDeviceSize textureSize = textureWidth * textureHeight * 4;
		uint32_t mipMapLevel = static_cast<uint32_t>(std::floor(std::log2(std::max(64, 64)))) + 1;
		cubeTextureMipMapLevel = mipMapLevel;
		if (!pixels)
			throw std::runtime_error("Problem!");

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		bufferHandler->CreateBuffer(textureSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device, physicalDevice);

		void* data;
		vkMapMemory(device, stagingBufferMemory, 0, textureSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(textureSize));
		vkUnmapMemory(device, stagingBufferMemory);

		stbi_image_free(pixels);

		CreateImage(physicalDevice, device, bufferHandler, textureWidth, textureHeight, VK_FORMAT_R8G8B8A8_SRGB,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory,isCubeMap, VK_SAMPLE_COUNT_1_BIT,mipMapLevel);

		bufferHandler->TransitionImageLayout(device, graphicsQueue, commandPool, textureImage,
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,isCubeMap,mipMapLevel);

		bufferHandler->CopyBufferToImage(device, commandPool, graphicsQueue, stagingBuffer,
			textureImage, static_cast<uint32_t>(textureWidth), static_cast<uint32_t>(textureHeight),isCubeMap);

		bufferHandler->TransitionImageLayout(device, graphicsQueue, commandPool, textureImage,
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,isCubeMap,mipMapLevel);

		vkDestroyBuffer(device, stagingBuffer, nullptr);
		vkFreeMemory(device, stagingBufferMemory, nullptr);

		//if(isCubeMap)
			//bufferHandler->GenerateMipMaps(device, commandPool, graphicsQueue, textureImage, 64, 64, mipMapLevel);
	}
	void InitializeTextureImageView(VkDevice device, VkImageAspectFlags aspectFlags, VkImage& textureImage, VkImageView& textureImageView,bool isCubeMap,uint32_t mipmapLevel = 1)
	{
		textureImageView = CreateImageView(device, textureImage, VK_FORMAT_R8G8B8A8_SRGB, aspectFlags, textureImageView,isCubeMap,mipmapLevel);
	}

	void InitializeSamplers(VkDevice device, VkSampler& textureSampler,bool isCubeMap,uint32_t mipmapLevel)
	{
		VkSamplerCreateInfo samplerInfo = {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = 16;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		//if (isCubeMap)
			//samplerInfo.maxLod = static_cast<float>(mipmapLevel);

		if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS)
			throw std::runtime_error("Problem!");
	}
	

	~Texture()
	{
		vkDestroyImageView(*deviceRef, depthImageView, nullptr);
		vkDestroyImage(*deviceRef, depthImage, nullptr);
		vkFreeMemory(*deviceRef, depthImageMemory, nullptr);

		vkDestroySampler(*deviceRef, textureSampler, nullptr);
		vkDestroySampler(*deviceRef, textureSampler2, nullptr);
		vkDestroySampler(*deviceRef, textureSamplerNormal, nullptr);
		vkDestroySampler(*deviceRef, textureSamplerMetallic, nullptr);
		vkDestroySampler(*deviceRef, textureSamplerRoughness, nullptr);
		vkDestroySampler(*deviceRef, textureSamplerAO, nullptr);
		vkDestroyImageView(*deviceRef, textureImageView, nullptr);
		vkDestroyImageView(*deviceRef, textureImageView2, nullptr);
		vkDestroyImageView(*deviceRef, textureImageViewNormal, nullptr);
		vkDestroyImageView(*deviceRef, textureImageMetallic, nullptr);
		vkDestroyImageView(*deviceRef, textureImageRoughness, nullptr);
		vkDestroyImageView(*deviceRef, textureImageAO, nullptr);
		vkDestroyImage(*deviceRef, textureImage, nullptr);
		vkDestroyImage(*deviceRef, textureImage2, nullptr);
		vkDestroyImage(*deviceRef, textureImageNormal, nullptr);
		vkDestroyImage(*deviceRef, textureImageMetallic, nullptr);
		vkDestroyImage(*deviceRef, textureImageRoughness, nullptr);
		vkDestroyImage(*deviceRef, textureImageAO, nullptr);
		vkFreeMemory(*deviceRef, textureImageMemory, nullptr);
		vkFreeMemory(*deviceRef, textureImageMemory2, nullptr);
		vkFreeMemory(*deviceRef, textureImageMemoryNormal, nullptr);
		vkFreeMemory(*deviceRef, textureImageMemoryMetallic, nullptr);
		vkFreeMemory(*deviceRef, textureImageMemoryRoughness, nullptr);
		vkFreeMemory(*deviceRef, textureImageMemoryAO, nullptr);
	}
};