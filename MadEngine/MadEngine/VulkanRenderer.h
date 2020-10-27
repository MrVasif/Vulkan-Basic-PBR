#ifndef VULKANRENDERER
#define VULKANRENDERER
#include "Instance.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "Surface.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include "GraphicsPipeline.h"
#include "Framebuffer.h"
#include "CommandPool.h"
#include "RenderLoop.h"
#include "BufferHandler.h"
#include "DescriptorSet.h"
#include "Texture.h"
#include "ObjectLoader.h"

struct GLFWInfo
{
public:
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = nullptr;
public:
	GLFWInfo()
	{
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	};
};

class VulkanRenderer
{
private:

	Instance* instance = new Instance;
	PhysicalDevice* device = new PhysicalDevice;
	LogicalDevice* logicalDevice = new LogicalDevice;
	Surface* surface = new Surface;
	SwapChain* swapchain = new SwapChain;
	RenderPass* renderPass = new RenderPass;
	GraphicsPipeline* graphicsPipeline = new GraphicsPipeline;
	Framebuffer* framebuffer = new Framebuffer;
	CommandPool* commandPool = new CommandPool;
	RenderLoop* renderLoop = new RenderLoop;
	Buffer* bufferHandler = new Buffer;
	DescriptorSet* descriptor = new DescriptorSet;
	Texture* textureLoader = new Texture;
	ObjectLoader* objectLoader = new ObjectLoader;
	ObjectLoader* objectLoader2 = new ObjectLoader;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	std::vector<Vertex> vertices2;
	std::vector<uint32_t> indices2;

	const std::string MODEL_PATH = "sphere.obj";
	const std::string MODEL_PATH2 = "sky.obj";
	const std::string TEXTURE_PATH = "Forest.jpg";
	const std::string TEXTURE_PATH2 = "DroidAlbedo.png";
	const std::string TEXTURE_PATH_NORMAL = "DroidNormal.png";
	const std::string TEXTURE_PATH_METALLIC = "DroidMetallic.png";
	const std::string TEXTURE_PATH_ROUGHNESS = "DroidRoughness.png";
	const std::string TEXTURE_PATH_AMBIENT_OCCLUSION = "DroidAO.png";
	const std::string CubeTex = "1.png";
public:
	void Start(GLFWwindow* window)
	{

		GLFWInfo glfwInfo;

		instance->Initialize(glfwInfo.glfwExtensionCount, glfwInfo.glfwExtensions);

		surface->Initialize(instance->instance, window, glfwGetWin32Window(window), nullptr);

		device->Initialize(instance->instance, &(surface->surface));

		logicalDevice->Initialize(device->physicalDevice, device->deviceExtensions, device->graphicsQueueFamily, device->presentQueueFamily);

		swapchain->Initialize(logicalDevice->logicalDevice, surface->surface, device->GetSwapChainSupportDetails(), device->graphicsQueueFamily, device->presentQueueFamily);

		renderPass->Initialize(swapchain->swapChainImageFormat, logicalDevice->logicalDevice, device->physicalDevice, textureLoader,device->msaaSamples);

		///Initialize Seperate Descriptor Set Layout
		//1
		descriptor->InitializeDescriptorSetLayout(logicalDevice->logicalDevice, descriptor->descriptorSetLayout);

		//PBR Model Descriptor Set
		//descriptor->InitializeDescriptorSetLayout(logicalDevice->logicalDevice, descriptor->descriptorSetLayout2);
		descriptor->InitializeDescriptorSetLayoutPBRModel(logicalDevice->logicalDevice, descriptor->descriptorSetLayout2);
		///

		///Initialize Seperate Pipeline
		//1
		graphicsPipeline->Initialize("vert.spv", "frag.spv", logicalDevice->logicalDevice, swapchain->swapChainExtent, renderPass->renderPass, descriptor->descriptorSetLayout, device->pushConstant, graphicsPipeline->pipelineLayout,graphicsPipeline->graphicsPipeline,device->msaaSamples);

		//2
		graphicsPipeline->Initialize("PbrVert.spv", "PbrFrag.spv", logicalDevice->logicalDevice, swapchain->swapChainExtent, renderPass->renderPass, descriptor->descriptorSetLayout2, device->pushConstant, graphicsPipeline->pipelineLayout2,graphicsPipeline->graphicsPipeline2,device->msaaSamples);

		///
		
		commandPool->InitializeCommandPool(logicalDevice->logicalDevice, device->graphicsQueueFamily);

		//-----Irradiance Cube Pass

		/*
		textureLoader->InitializeIrradianceCubeImage(device->physicalDevice, logicalDevice->logicalDevice, bufferHandler);

		renderPass->InitializeIrradiancePass(logicalDevice->logicalDevice, device->physicalDevice);

		framebuffer->InitializeOffScreenFramebuffer(logicalDevice->logicalDevice, renderPass->irradianceCubeRenderPass, textureLoader->offscreenImageView);
		*/

		//-------------------------------------------------------------------------------------------------------------

		textureLoader->InitializeColorResources(device->physicalDevice, logicalDevice->logicalDevice, bufferHandler, swapchain->swapChainExtent, device->msaaSamples);

		textureLoader->InitializeDepthBuffer(device->physicalDevice, logicalDevice->logicalDevice, swapchain->swapChainExtent, bufferHandler,device->msaaSamples);

		framebuffer->Initialize(textureLoader->depthImageView,textureLoader->colorImageView, swapchain->swapchainImageViews, renderPass->renderPass, swapchain->swapChainExtent, logicalDevice->logicalDevice);

		///Seperate Texture Settings
		//1
		textureLoader->InitializeTextures(logicalDevice->logicalDevice, logicalDevice->graphicsQueue, commandPool->commandPool, device->physicalDevice, bufferHandler,TEXTURE_PATH, textureLoader->textureImage, textureLoader->textureImageMemory);
		textureLoader->InitializeTextureImageView(logicalDevice->logicalDevice, VK_IMAGE_ASPECT_COLOR_BIT, textureLoader->textureImage, textureLoader->textureImageView,false);
		textureLoader->InitializeSamplers(logicalDevice->logicalDevice, textureLoader->textureSampler,false,0);
		//PBR Textures

		//Albedo
		textureLoader->InitializeTextures(logicalDevice->logicalDevice, logicalDevice->graphicsQueue, commandPool->commandPool, device->physicalDevice, bufferHandler,TEXTURE_PATH2, textureLoader->textureImage2, textureLoader->textureImageMemory2);
		textureLoader->InitializeTextureImageView(logicalDevice->logicalDevice, VK_IMAGE_ASPECT_COLOR_BIT, textureLoader->textureImage2, textureLoader->textureImageView2,false);
		textureLoader->InitializeSamplers(logicalDevice->logicalDevice, textureLoader->textureSampler2,false,0); 
		//Normal
		textureLoader->InitializeTextures(logicalDevice->logicalDevice, logicalDevice->graphicsQueue, commandPool->commandPool, device->physicalDevice, bufferHandler, TEXTURE_PATH_NORMAL, textureLoader->textureImageNormal, textureLoader->textureImageMemoryNormal);
		textureLoader->InitializeTextureImageView(logicalDevice->logicalDevice, VK_IMAGE_ASPECT_COLOR_BIT, textureLoader->textureImageNormal, textureLoader->textureImageViewNormal, false);
		textureLoader->InitializeSamplers(logicalDevice->logicalDevice, textureLoader->textureSamplerNormal,false,0);
		//Metallic
		textureLoader->InitializeTextures(logicalDevice->logicalDevice, logicalDevice->graphicsQueue, commandPool->commandPool, device->physicalDevice, bufferHandler, TEXTURE_PATH_METALLIC, textureLoader->textureImageMetallic, textureLoader->textureImageMemoryMetallic);
		textureLoader->InitializeTextureImageView(logicalDevice->logicalDevice, VK_IMAGE_ASPECT_COLOR_BIT, textureLoader->textureImageMetallic, textureLoader->textureImageViewMetallic, false);
		textureLoader->InitializeSamplers(logicalDevice->logicalDevice, textureLoader->textureSamplerMetallic,false,0);
		//Roughness
		textureLoader->InitializeTextures(logicalDevice->logicalDevice, logicalDevice->graphicsQueue, commandPool->commandPool, device->physicalDevice, bufferHandler, TEXTURE_PATH_ROUGHNESS, textureLoader->textureImageRoughness, textureLoader->textureImageMemoryRoughness);
		textureLoader->InitializeTextureImageView(logicalDevice->logicalDevice, VK_IMAGE_ASPECT_COLOR_BIT, textureLoader->textureImageRoughness, textureLoader->textureImageViewRoughness, false);
		textureLoader->InitializeSamplers(logicalDevice->logicalDevice, textureLoader->textureSamplerRoughness,false,0);
		//Ambient Occlusion
		textureLoader->InitializeTextures(logicalDevice->logicalDevice, logicalDevice->graphicsQueue, commandPool->commandPool, device->physicalDevice, bufferHandler, TEXTURE_PATH_AMBIENT_OCCLUSION, textureLoader->textureImageAO, textureLoader->textureImageMemoryAO);
		textureLoader->InitializeTextureImageView(logicalDevice->logicalDevice, VK_IMAGE_ASPECT_COLOR_BIT, textureLoader->textureImageAO, textureLoader->textureImageViewAO, false);
		textureLoader->InitializeSamplers(logicalDevice->logicalDevice, textureLoader->textureSamplerAO,false,0);
		//CubeMap
		textureLoader->InitializeTextures(logicalDevice->logicalDevice, logicalDevice->graphicsQueue, commandPool->commandPool, device->physicalDevice, bufferHandler, "ForestEdited2.jpg", textureLoader->irradianceCubeImage, textureLoader->irradianceCubeDeviceMemory,true);
		textureLoader->InitializeTextureImageView(logicalDevice->logicalDevice, VK_IMAGE_ASPECT_COLOR_BIT, textureLoader->irradianceCubeImage, textureLoader->irradianceCubeImageView, true,textureLoader->cubeTextureMipMapLevel);
		textureLoader->InitializeSamplers(logicalDevice->logicalDevice, textureLoader->irradianceCubeSampler,false,textureLoader->cubeTextureMipMapLevel);
		

		///Loading Objects to Buffers
		//1
		objectLoader->Initialize(MODEL_PATH, vertices, indices);
		//2
		objectLoader->Initialize("Droid.obj", vertices2, indices2);
		///

		///Initialize Buffers Accordingly
		//1
		bufferHandler->InitializeVertexBuffer(logicalDevice->logicalDevice, vertices, device->physicalDevice, commandPool->commandPool, logicalDevice->graphicsQueue, bufferHandler->vertexBuffer, bufferHandler->vertexBufferMemory);

		bufferHandler->InitializeIndexBuffer(logicalDevice->logicalDevice, indices, device->physicalDevice, commandPool->commandPool, logicalDevice->graphicsQueue, bufferHandler->indexBuffer, bufferHandler->indexBufferMemory);

		bufferHandler->InitializeUniformBuffer(logicalDevice->logicalDevice, device->physicalDevice, swapchain->swapChainImages, bufferHandler->uniformBuffers, bufferHandler->uniformBuffersMemory);

		//2
		bufferHandler->InitializeVertexBuffer(logicalDevice->logicalDevice, vertices2, device->physicalDevice, commandPool->commandPool, logicalDevice->graphicsQueue, bufferHandler->vertexBuffer2, bufferHandler->vertexBufferMemory2);

		bufferHandler->InitializeIndexBuffer(logicalDevice->logicalDevice, indices2, device->physicalDevice, commandPool->commandPool, logicalDevice->graphicsQueue, bufferHandler->indexBuffer2, bufferHandler->indexBufferMemory2);

		bufferHandler->InitializeUniformBuffer(logicalDevice->logicalDevice, device->physicalDevice, swapchain->swapChainImages, bufferHandler->uniformBuffers2, bufferHandler->uniformBuffersMemory2);
		///

		///Initialize DescriptorPool Seperately
		//1
		descriptor->InitializeDescriptorPool(logicalDevice->logicalDevice, swapchain->swapChainImages, descriptor->descriptorPool);

		//2
		descriptor->InitializeDescriptorPoolPBR(logicalDevice->logicalDevice, swapchain->swapChainImages, descriptor->descriptorPool2);
		///

		///Initialize Seperate Descriptor Sets
		//1
		descriptor->InitializeDescriptorSets(logicalDevice->logicalDevice, swapchain->swapChainImages, bufferHandler->uniformBuffers, textureLoader->textureImageView, textureLoader->textureSampler, descriptor->descriptorSets, descriptor->descriptorSetLayout, descriptor->descriptorPool);

		//2
		//descriptor->InitializeDescriptorSets(logicalDevice->logicalDevice, swapchain->swapChainImages, bufferHandler->uniformBuffers2, textureLoader->textureImageView2, textureLoader->textureSampler2, descriptor->descriptorSets2, descriptor->descriptorSetLayout2, descriptor->descriptorPool2);
		descriptor->InitializeDescriptorSetsPBR(logicalDevice->logicalDevice, swapchain->swapChainImages, bufferHandler->uniformBuffers2,descriptor->descriptorSets2, descriptor->descriptorSetLayout2, descriptor->descriptorPool2, textureLoader->textureImageView2, textureLoader->textureSampler2, 
			textureLoader->textureImageViewNormal,textureLoader->textureSamplerNormal,textureLoader->textureImageViewMetallic,textureLoader->textureSamplerMetallic,textureLoader->textureImageViewRoughness,textureLoader->textureSamplerRoughness,
			textureLoader->textureImageViewAO,textureLoader->textureSamplerAO,textureLoader->irradianceCubeImageView,textureLoader->irradianceCubeSampler);

		commandPool->InitializeCommandBuffer(framebuffer->swapChainFramebuffers, logicalDevice->logicalDevice, renderPass->renderPass, swapchain->swapChainExtent, graphicsPipeline->graphicsPipeline,graphicsPipeline->graphicsPipeline2, vertices,
			bufferHandler->vertexBuffer, bufferHandler->vertexBuffer2, bufferHandler->indexBuffer, bufferHandler->indexBuffer2, indices, indices2, graphicsPipeline->pipelineLayout, graphicsPipeline->pipelineLayout2, descriptor->descriptorSets, descriptor->descriptorSets2, device->pushConstant);

		renderLoop->InitializeSyncObjects(logicalDevice->logicalDevice, swapchain->swapChainImages);
		
	}

	void Update()
	{
		renderLoop->Initialize(logicalDevice->logicalDevice, swapchain->swapChain, commandPool->commandBuffers, logicalDevice->graphicsQueue, logicalDevice->presentQueue, swapchain->swapChainExtent, bufferHandler);
	}
};

#endif