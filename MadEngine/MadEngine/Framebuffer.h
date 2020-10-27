#pragma once
#include "Essentials.h"

class Framebuffer
{
public:
	std::vector<VkFramebuffer> swapChainFramebuffers;
private:
	VkDevice* deviceRef;
public:

	void Initialize(VkImageView depthImageView,VkImageView colorImageView,std::vector<VkImageView> swapchainImageViews,VkRenderPass renderPass,VkExtent2D swapchainExtent,VkDevice device)
	{
		deviceRef = &device;
		swapChainFramebuffers.resize(swapchainImageViews.size());

		for (size_t i = 0; i < swapchainImageViews.size(); i++)
		{
			std::array<VkImageView,3> attachments = { colorImageView,depthImageView,swapchainImageViews[i] };

			VkFramebufferCreateInfo frameBufferInfo = {};
			frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			frameBufferInfo.renderPass = renderPass;
			frameBufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());			
			frameBufferInfo.pAttachments = attachments.data();
			frameBufferInfo.width = swapchainExtent.width;
			frameBufferInfo.height = swapchainExtent.height;
			frameBufferInfo.layers = 1; // Means that single image

			if (vkCreateFramebuffer(device, &frameBufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
				throw std::runtime_error("Problem!");
		}

	}

	void InitializeWithoutDepth(std::vector<VkImageView> swapchainImageViews, VkRenderPass renderPass, VkExtent2D swapchainExtent, VkDevice device)
	{
		deviceRef = &device;
		swapChainFramebuffers.resize(swapchainImageViews.size());

		for (size_t i = 0; i < swapchainImageViews.size(); i++)
		{

			VkFramebufferCreateInfo frameBufferInfo = {};
			frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			frameBufferInfo.renderPass = renderPass;
			frameBufferInfo.attachmentCount = static_cast<uint32_t>(sizeof(swapchainImageViews[i]) / sizeof(VkImageView));
			frameBufferInfo.pAttachments = &swapchainImageViews[i];
			frameBufferInfo.width = swapchainExtent.width;
			frameBufferInfo.height = swapchainExtent.height;
			frameBufferInfo.layers = 1; // Means that single image

			if (vkCreateFramebuffer(device, &frameBufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
				throw std::runtime_error("Problem!");
		}

	}

	~Framebuffer()
	{
		for (auto framebuffer : swapChainFramebuffers)
			vkDestroyFramebuffer(*deviceRef, framebuffer, nullptr);
	}
};