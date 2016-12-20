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

	// デバイス作成
	{
		vk::DeviceQueueCreateInfo queueInfo = {};

		m_gpu[0].getQueueFamilyProperties(&m_queueFamilyCount, nullptr);
		assert(m_queueFamilyCount >= 1);
		
		m_queueProps.resize(m_queueFamilyCount);
		m_gpu[0].getQueueFamilyProperties(&m_queueFamilyCount, m_queueProps.data());
		assert(m_queueFamilyCount >= 1);

		// 使用するグラフィックスキューを探す
		bool found = false;
		for (uint32_t i = 0; i < m_queueFamilyCount; i++) {
			if (m_queueProps[i].queueFlags & vk::QueueFlagBits::eGraphics) {
				queueInfo.queueFamilyIndex = i;
				m_graphicsQueueFamilyIndex = i;
				found = true;
				break;
			}
		}
		assert(found);

		float queue_priorities[1] = { 0.0 };
		//queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.pNext = NULL;
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = queue_priorities;

		vk::DeviceCreateInfo deviceInfo = {};
		deviceInfo.queueCreateInfoCount = 1;
		deviceInfo.pQueueCreateInfos = &queueInfo;
		deviceInfo.enabledExtensionCount = 0;
		deviceInfo.ppEnabledExtensionNames = nullptr;
		deviceInfo.enabledLayerCount = 0;
		deviceInfo.ppEnabledLayerNames = nullptr;
		deviceInfo.pEnabledFeatures = nullptr;

		vk::Result res = m_gpu[0].createDevice(&deviceInfo, nullptr, &m_device);
		assert(res == vk::Result::eSuccess);
	}

	// コマンドプール
	{
		vk::CommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.pNext = nullptr;
		cmdPoolInfo.queueFamilyIndex = m_graphicsQueueFamilyIndex;
		cmdPoolInfo.flags = vk::CommandPoolCreateFlags();

		vk::Result res = m_device.createCommandPool(&cmdPoolInfo, nullptr, &m_commandPool);
		assert(res == vk::Result::eSuccess);
	}

	// コマンドバッファ
	{
		vk::CommandBufferAllocateInfo cmd = {};
		cmd.pNext = nullptr;
		cmd.commandPool = m_commandPool;
		cmd.level = vk::CommandBufferLevel::ePrimary;
		cmd.commandBufferCount = 1;

		vk::Result res = m_device.allocateCommandBuffers(&cmd, &m_commandBuffer);
		assert(res == vk::Result::eSuccess);
	}
}

void SimpleVulkan::Cleanup()
{
	vk::CommandBuffer cmdBufs[1] = { m_commandBuffer };
	m_device.freeCommandBuffers(m_commandPool, 1, cmdBufs);

	m_device.destroyCommandPool(m_commandPool);
	m_device.destroy();
	m_vkInstance.destroy();
}