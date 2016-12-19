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
	// Vulkanインスタンス作成
	{
		// アプリケーション情報作成
		vk::ApplicationInfo app_info = {};
		//app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pNext = NULL;
		app_info.pApplicationName = APP_SHORT_NAME;
		app_info.applicationVersion = 1;
		app_info.pEngineName = APP_SHORT_NAME;
		app_info.engineVersion = 1;
		app_info.apiVersion = VK_API_VERSION_1_0;


		// Extension
		const char* extensions[] = {
			VK_KHR_SURFACE_EXTENSION_NAME,
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,				// Windows用Extension
#if defined(_DEBUG)
			VK_EXT_DEBUG_REPORT_EXTENSION_NAME,					// デバッグレポート用Extension
#endif
		};

		// Layer
		const char* layers[] = {
			"VK_LAYER_LUNARG_standard_validation"
		};

		// インスタンス情報作成
		vk::InstanceCreateInfo inst_info = {};
		//inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		inst_info.pNext = NULL;
		inst_info.flags = vk::InstanceCreateFlags();
		inst_info.pApplicationInfo = &app_info;
		inst_info.enabledExtensionCount = ARRAYSIZE(extensions);
		inst_info.ppEnabledExtensionNames = extensions;
#if defined(_DEBUG)
		inst_info.enabledLayerCount = ARRAYSIZE(layers);
		inst_info.ppEnabledLayerNames = layers;
#endif

		try {
			m_vkInstance = vk::createInstance(inst_info);
		}
		catch (const std::exception& e) {
			std::cout << "Could not create a Vulkan instance: " << e.what() << std::endl;
			//return 1;
		}
	}

	// 物理デバイス取得
	{
		m_gpuCount = 1;

		vk::Result res = m_vkInstance.enumeratePhysicalDevices(&m_gpuCount, nullptr);
		assert(m_gpuCount >= 1);

		m_gpu.resize(m_gpuCount);
		res = m_vkInstance.enumeratePhysicalDevices(&m_gpuCount, m_gpu.data());
		assert(res == vk::Result::eSuccess && m_gpuCount >= 1);
	}
}

void SimpleVulkan::Cleanup()
{
	m_vkInstance.destroy();
}