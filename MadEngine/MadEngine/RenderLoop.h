#pragma once
#include "BufferHandler.h"
class RenderLoop
{
private:
	const int MAX_CONCURRENT_FRAME_COUNT = 2;
	size_t currentFrame = 0;
	std::vector<VkSemaphore> imageAvaibleSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> fences;
	std::vector<VkFence> imagesInFlight;
	VkDevice* deviceRef;
public:
	void InitializeSyncObjects(VkDevice device, std::vector<VkImage> swapchainImages)
	{
		deviceRef = &device;

		imageAvaibleSemaphores.resize(MAX_CONCURRENT_FRAME_COUNT);
		renderFinishedSemaphores.resize(MAX_CONCURRENT_FRAME_COUNT);
		fences.resize(MAX_CONCURRENT_FRAME_COUNT);
		imagesInFlight.resize(swapchainImages.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_CONCURRENT_FRAME_COUNT; i++)
		{
			if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvaibleSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(device, &fenceInfo, nullptr, &fences[i]) != VK_SUCCESS)
				throw std::runtime_error("Problem!");
		}


	}

	void Initialize(VkDevice device, VkSwapchainKHR swapchain, std::vector<VkCommandBuffer> commandBuffers,
		VkQueue graphicsQueue, VkQueue presentQueue, VkExtent2D extent, Buffer* uniformBuffer)
	{
		vkWaitForFences(device, 1, &fences[currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex = 0;
		vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAvaibleSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

		uniformBuffer->UpdateUniformBuffer(device, extent, imageIndex);

		if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
			vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		imagesInFlight[imageIndex] = fences[currentFrame];

		// Submit
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore waitSemaphores[] = { imageAvaibleSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(device, 1, &fences[currentFrame]);

		if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, fences[currentFrame]) != VK_SUCCESS)
			throw std::runtime_error("Problem!");

		// Get Back
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		VkSwapchainKHR swapChains[] = { swapchain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(presentQueue, &presentInfo);

		currentFrame = (currentFrame + 1) % MAX_CONCURRENT_FRAME_COUNT;
	}


	~RenderLoop()
	{
		for (size_t i = 0; i < MAX_CONCURRENT_FRAME_COUNT; i++)
		{
			vkDestroySemaphore(*deviceRef, renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(*deviceRef, imageAvaibleSemaphores[i], nullptr);
			vkDestroyFence(*deviceRef, fences[i], nullptr);
		}

	}
};