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
	// Vulkan�C���X�^���X�쐬
	{
		// �A�v���P�[�V�������쐬
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
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,				// Windows�pExtension
#if defined(_DEBUG)
			VK_EXT_DEBUG_REPORT_EXTENSION_NAME,					// �f�o�b�O���|�[�g�pExtension
#endif
		};

		// Layer
		const char* layers[] = {
			"VK_LAYER_LUNARG_standard_validation"
		};

		// �C���X�^���X���쐬
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

	// �����f�o�C�X�擾
	{
		m_gpuCount = 1;

		vk::Result res = m_vkInstance.enumeratePhysicalDevices(&m_gpuCount, nullptr);
		assert(m_gpuCount >= 1);

		m_gpu.resize(m_gpuCount);
		res = m_vkInstance.enumeratePhysicalDevices(&m_gpuCount, m_gpu.data());
		assert(res == vk::Result::eSuccess && m_gpuCount >= 1);
	}

	// �f�o�C�X�쐬
	{
		vk::DeviceQueueCreateInfo queueInfo = {};

		m_gpu[0].getQueueFamilyProperties(&m_queueFamilyCount, nullptr);
		assert(m_queueFamilyCount >= 1);
		
		m_queueProps.resize(m_queueFamilyCount);
		m_gpu[0].getQueueFamilyProperties(&m_queueFamilyCount, m_queueProps.data());
		assert(m_queueFamilyCount >= 1);

		// �g�p����O���t�B�b�N�X�L���[��T��
		bool found = false;
		for (int i = 0; i < m_queueFamilyCount; i++) {
			if (m_queueProps[i].queueFlags & vk::QueueFlagBits::eGraphics) {
				queueInfo.queueFamilyIndex = i;
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
}

void SimpleVulkan::Cleanup()
{
	m_device.destroy();
	m_vkInstance.destroy();
}