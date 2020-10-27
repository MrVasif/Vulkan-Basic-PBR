#pragma once
#include  "Essentials.h"

class CommandPool
{
public:
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
private:
	VkDevice* deviceRef;
public:	
	void InitializeCommandPool(VkDevice device, uint32_t graphicsFamilyQueueIndex)
	{
		//Creating Command Pool
		deviceRef = &device;
		VkCommandPoolCreateInfo commandPoolInfo = {};
		commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolInfo.queueFamilyIndex = graphicsFamilyQueueIndex;
		commandPoolInfo.flags = 0;

		if (vkCreateCommandPool(device, &commandPoolInfo, nullptr, &commandPool) != VK_SUCCESS)
			throw std::runtime_error("Problem!");
	}

	void InitializeCommandBuffer(std::vector<VkFramebuffer> swapChainFramebuffers, VkDevice device, VkRenderPass renderpass,
		VkExtent2D swapchainExtent, VkPipeline graphicsPipeline,VkPipeline graphicsPipeline2, std::vector<Vertex> vertices,
		VkBuffer vertexBuffer,VkBuffer vertexBuffer2, VkBuffer indexBuffer,VkBuffer indexBuffer2, const std::vector<uint32_t> indices,const std::vector<uint32_t> indices2,
		VkPipelineLayout pipelineLayout,VkPipelineLayout pipelineLayout2,std::vector <VkDescriptorSet> descriptorSets, std::vector <VkDescriptorSet> descriptorSets2, float& pushConstant)
	{
		//Creating CommandBuffers
		commandBuffers.resize(swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandPool = commandPool;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocateInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		if (vkAllocateCommandBuffers(device, &allocateInfo, commandBuffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Problem!");

		for (size_t i = 0; i < commandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo commandBufferBeginInfo = {};
			commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			commandBufferBeginInfo.flags = 0;
			commandBufferBeginInfo.pInheritanceInfo = 0;

			if (vkBeginCommandBuffer(commandBuffers[i], &commandBufferBeginInfo) != VK_SUCCESS)
				throw std::runtime_error("Problem!");

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = renderpass;
			renderPassInfo.framebuffer = swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = swapchainExtent;
			std::array<VkClearValue, 3> clearValues = {};
			clearValues[0].color = { 0.0f,0.0f,0.0f,1.0f };
			clearValues[1].depthStencil = { 1.0f,0 };
			clearValues[2].color = { 0.0f,0.0f,0.0f,1.0f };
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			pushConstant = 1.0f;
			VkBuffer  vertexBuffers[] = { vertexBuffer};
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
			vkCmdPushConstants(commandBuffers[i], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pushConstant), &pushConstant);
			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
			vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT32);
			vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);
			vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

			vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline2);
			vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout2, 0, 1, &descriptorSets2[i], 0, nullptr);
			vkCmdPushConstants(commandBuffers[i], pipelineLayout2, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pushConstant), &pushConstant);
			VkBuffer vertexBuffers2[] = { vertexBuffer2 };
			VkDeviceSize offsets2[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers2, offsets2);
			vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer2, 0, VK_INDEX_TYPE_UINT32);
			vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices2.size()), 1, 0, 0, 0);
			vkCmdEndRenderPass(commandBuffers[i]);

			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
				throw std::runtime_error("Problen!");
		}
	}

	~CommandPool()
	{
		vkDestroyCommandPool(*deviceRef, commandPool, nullptr);
	}
};