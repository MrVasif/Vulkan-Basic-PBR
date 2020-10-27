#pragma once
#include "Essentials.h"

class DescriptorSet
{
public:
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorSetLayout descriptorSetLayout2;
	VkDescriptorPool descriptorPool;
	VkDescriptorPool descriptorPool2;
	std::vector <VkDescriptorSet> descriptorSets;
	std::vector<VkDescriptorSet> descriptorSets2;
private:
	VkDevice* deviceRef;
public:
	void InitializeDescriptorSetLayout(VkDevice device,VkDescriptorSetLayout& descriptorSetLayout)
	{
		deviceRef = &device;

		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding.pImmutableSamplers = nullptr;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding,samplerLayoutBinding};
		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
			throw std::runtime_error("Problem!");
	}

	void InitializeDescriptorSetLayoutPBRModel(VkDevice device, VkDescriptorSetLayout& descriptorSetLayout)
	{
		deviceRef = &device;

		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding samplerLayoutBinding2 = {};
		samplerLayoutBinding2.binding = 2;
		samplerLayoutBinding2.descriptorCount = 1;
		samplerLayoutBinding2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding2.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding2.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding samplerLayoutBinding3 = {};
		samplerLayoutBinding3.binding = 3;
		samplerLayoutBinding3.descriptorCount = 1;
		samplerLayoutBinding3.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding3.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding3.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding samplerLayoutBinding4 = {};
		samplerLayoutBinding4.binding = 4;
		samplerLayoutBinding4.descriptorCount = 1;
		samplerLayoutBinding4.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding4.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding4.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding samplerLayoutBinding5 = {};
		samplerLayoutBinding5.binding = 5;
		samplerLayoutBinding5.descriptorCount = 1;
		samplerLayoutBinding5.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding5.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding5.pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutBinding samplerLayoutBinding6 = {};
		samplerLayoutBinding6.binding = 6;
		samplerLayoutBinding6.descriptorCount = 1;
		samplerLayoutBinding6.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding6.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		samplerLayoutBinding6.pImmutableSamplers = nullptr;

		std::array<VkDescriptorSetLayoutBinding, 7> bindings = { uboLayoutBinding,samplerLayoutBinding,samplerLayoutBinding2,samplerLayoutBinding3,samplerLayoutBinding4,samplerLayoutBinding5,samplerLayoutBinding6 };
		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
			throw std::runtime_error("Problem!");
	}

	void InitializeDescriptorPool(VkDevice device, std::vector<VkImage> swapchainImages,VkDescriptorPool& descriptorPool)
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes = {};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(swapchainImages.size());
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;	
		poolSizes[1].descriptorCount = static_cast<uint32_t>(swapchainImages.size());

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(swapchainImages.size());

		if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
			throw std::runtime_error("Problem!");
	}

	void InitializeDescriptorPoolPBR(VkDevice device, std::vector<VkImage> swapchainImages, VkDescriptorPool& descriptorPool)
	{
		std::array<VkDescriptorPoolSize, 7> poolSizes = {};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(swapchainImages.size());
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(swapchainImages.size());
		poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[2].descriptorCount = static_cast<uint32_t>(swapchainImages.size());
		poolSizes[3].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[3].descriptorCount = static_cast<uint32_t>(swapchainImages.size());
		poolSizes[4].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[4].descriptorCount = static_cast<uint32_t>(swapchainImages.size());
		poolSizes[5].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[5].descriptorCount = static_cast<uint32_t>(swapchainImages.size());
		poolSizes[6].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[6].descriptorCount = static_cast<uint32_t>(swapchainImages.size());

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(swapchainImages.size());

		if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
			throw std::runtime_error("Problem!");
	}

	void InitializeDescriptorSets(VkDevice device, std::vector<VkImage> swapchainImages, std::vector<VkBuffer> uniformBuffers,
		VkImageView textureImageView, VkSampler textureSampler,std::vector<VkDescriptorSet>& descriptorSets,VkDescriptorSetLayout& descriptorSetLayout,VkDescriptorPool descriptorPool)
	{
		std::vector<VkDescriptorSetLayout> layouts(swapchainImages.size(), descriptorSetLayout);

		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = descriptorPool;
		allocateInfo.descriptorSetCount = static_cast<uint32_t>(swapchainImages.size());
		allocateInfo.pSetLayouts = layouts.data();

		descriptorSets.resize(swapchainImages.size());

		if (vkAllocateDescriptorSets(device, &allocateInfo, descriptorSets.data()) != VK_SUCCESS)
			throw std::runtime_error("Problem!");

		for (size_t i = 0; i < swapchainImages.size(); i++)
		{
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = uniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = textureImageView;
			imageInfo.sampler = textureSampler;

			std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = descriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = descriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}

	void InitializeDescriptorSetsPBR(VkDevice device, std::vector<VkImage> swapchainImages, std::vector<VkBuffer> uniformBuffers,
		std::vector<VkDescriptorSet>& descriptorSets, VkDescriptorSetLayout& descriptorSetLayout, VkDescriptorPool descriptorPool, VkImageView textureImageViewAlbedo, VkSampler textureSamplerAlbedo,
		VkImageView textureImageViewNormal, VkSampler textureSamplerNormal, VkImageView textureImageViewMetallic, VkSampler textureSamplerMetallic, VkImageView textureImageViewRoughness, VkSampler textureSamplerRoughness,
		VkImageView textureImageViewAO, VkSampler textureSamplerAO,VkImageView irradianceImageView,VkSampler irradianceSampler)
	{
		std::vector<VkDescriptorSetLayout> layouts(swapchainImages.size(), descriptorSetLayout);

		VkDescriptorSetAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = descriptorPool;
		allocateInfo.descriptorSetCount = static_cast<uint32_t>(swapchainImages.size());
		allocateInfo.pSetLayouts = layouts.data();

		descriptorSets.resize(swapchainImages.size());

		if (vkAllocateDescriptorSets(device, &allocateInfo, descriptorSets.data()) != VK_SUCCESS)
			throw std::runtime_error("Problem!");

		for (size_t i = 0; i < swapchainImages.size(); i++)
		{
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = uniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorImageInfo imageInfoAlbedo = {};
			imageInfoAlbedo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfoAlbedo.imageView = textureImageViewAlbedo;
			imageInfoAlbedo.sampler = textureSamplerAlbedo;

			VkDescriptorImageInfo imageInfoNormal = {};
			imageInfoNormal.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfoNormal.imageView = textureImageViewNormal;
			imageInfoNormal.sampler = textureSamplerNormal;

			VkDescriptorImageInfo imageInfoMetallic = {};
			imageInfoMetallic.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfoMetallic.imageView = textureImageViewMetallic;
			imageInfoMetallic.sampler = textureSamplerMetallic;

			VkDescriptorImageInfo imageInfoRoughness = {};
			imageInfoRoughness.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfoRoughness.imageView = textureImageViewRoughness;
			imageInfoRoughness.sampler = textureSamplerRoughness;

			VkDescriptorImageInfo imageInfoAO = {};
			imageInfoAO.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfoAO.imageView = textureImageViewAO;
			imageInfoAO.sampler = textureSamplerAO;

			VkDescriptorImageInfo imageInfoIrradiance = {};
			imageInfoIrradiance.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfoIrradiance.imageView = irradianceImageView;
			imageInfoIrradiance.sampler = irradianceSampler;

			std::array<VkWriteDescriptorSet, 7> descriptorWrites = {};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = descriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = descriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].pImageInfo = &imageInfoAlbedo;

			descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[2].dstSet = descriptorSets[i];
			descriptorWrites[2].dstBinding = 2;
			descriptorWrites[2].dstArrayElement = 0;
			descriptorWrites[2].descriptorCount = 1;
			descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[2].pImageInfo = &imageInfoNormal;

			descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[3].dstSet = descriptorSets[i];
			descriptorWrites[3].dstBinding = 3;
			descriptorWrites[3].dstArrayElement = 0;
			descriptorWrites[3].descriptorCount = 1;
			descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[3].pImageInfo = &imageInfoMetallic;

			descriptorWrites[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[4].dstSet = descriptorSets[i];
			descriptorWrites[4].dstBinding = 4;
			descriptorWrites[4].dstArrayElement = 0;
			descriptorWrites[4].descriptorCount = 1;
			descriptorWrites[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[4].pImageInfo = &imageInfoRoughness;

			descriptorWrites[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[5].dstSet = descriptorSets[i];
			descriptorWrites[5].dstBinding = 5;
			descriptorWrites[5].dstArrayElement = 0;
			descriptorWrites[5].descriptorCount = 1;
			descriptorWrites[5].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[5].pImageInfo = &imageInfoAO;

			descriptorWrites[6].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[6].dstSet = descriptorSets[i];
			descriptorWrites[6].dstBinding = 6;
			descriptorWrites[6].dstArrayElement = 0;
			descriptorWrites[6].descriptorCount = 1;
			descriptorWrites[6].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[6].pImageInfo = &imageInfoIrradiance;

			vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}

	~DescriptorSet()
	{
		vkDestroyDescriptorPool(*deviceRef, descriptorPool, nullptr);
		vkDestroyDescriptorPool(*deviceRef, descriptorPool2, nullptr);
		vkDestroyDescriptorSetLayout(*deviceRef, descriptorSetLayout, nullptr);
		vkDestroyDescriptorSetLayout(*deviceRef, descriptorSetLayout2, nullptr);
	}
};