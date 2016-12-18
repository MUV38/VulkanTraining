#include <iostream>

#include "SimpleVulkan.h"

#define APP_SHORT_NAME "vulkansamples_instance"

SimpleVulkan::SimpleVulkan()
{
}

SimpleVulkan::~SimpleVulkan()
{
}


void SimpleVulkan::Init()
{
	// initialize the VkApplicationInfo structure
	vk::ApplicationInfo app_info = {};
	//app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = NULL;
	app_info.pApplicationName = APP_SHORT_NAME;
	app_info.applicationVersion = 1;
	app_info.pEngineName = APP_SHORT_NAME;
	app_info.engineVersion = 1;
	app_info.apiVersion = VK_API_VERSION_1_0;

	// initialize the VkInstanceCreateInfo structure
	vk::InstanceCreateInfo inst_info = {};
	//inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_info.pNext = NULL;
	inst_info.flags = vk::InstanceCreateFlags();
	inst_info.pApplicationInfo = &app_info;
	inst_info.enabledExtensionCount = 0;
	inst_info.ppEnabledExtensionNames = NULL;
	inst_info.enabledLayerCount = 0;
	inst_info.ppEnabledLayerNames = NULL;

	try {
		m_vkInstance = vk::createInstance(inst_info);
	}
	catch (const std::exception& e) {
		std::cout << "Could not create a Vulkan instance: " << e.what() << std::endl;
		//return 1;
	}
}

void SimpleVulkan::Cleanup()
{
	m_vkInstance.destroy();
}