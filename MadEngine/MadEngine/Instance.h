#pragma once
#include "Essentials.h"

class Instance
{
public:
	VkInstance instance;

	const std::vector<const char*> validationLayers =
	{
		"VK_LAYER_KHRONOS_validation"
	};

#ifdef _DEBUG
	const bool debugModeEnabled = true;
#else
	const bool debugModeEnabled = false;
#endif // DEBUG

public:
	bool CheckLayerSupport()
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> avaibleLayers(layerCount);

		vkEnumerateInstanceLayerProperties(&layerCount, avaibleLayers.data());

		for (const char* validationLayer : validationLayers)
		{
			bool isLayerSupported = false;
			for (const auto& layer : avaibleLayers)
			{
				if (strcmp(validationLayer, layer.layerName) == 0)
				{
					isLayerSupported = true;
					break;
				}
			}

			if (!isLayerSupported)
				return false;
		}

		return true;
	}
	void Initialize(uint32_t extensionCount, const char** extensionNames)
	{
		if(debugModeEnabled && !CheckLayerSupport())
			throw std::runtime_error("Problem! ");

		VkApplicationInfo applicationInfo = {};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pNext = nullptr;
		applicationInfo.pApplicationName = "Mad Renderer";
		applicationInfo.pEngineName = "Mad Engine";
		applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
		applicationInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pNext = nullptr;
		instanceInfo.pApplicationInfo = &applicationInfo;
		if (debugModeEnabled)
		{
			instanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			instanceInfo.ppEnabledLayerNames = validationLayers.data();
		}
		instanceInfo.enabledExtensionCount = extensionCount;
		instanceInfo.ppEnabledExtensionNames = extensionNames;

		if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS)
			throw std::runtime_error("Problem");
	}

	~Instance()
	{
		vkDestroyInstance(instance, nullptr);
	}
};