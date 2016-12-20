#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#include <vulkan/vulkan.hpp>

class SimpleVulkan
{
public:
	SimpleVulkan();
	~SimpleVulkan();

	void Init();
	void Cleanup();

private:
	vk::Instance						m_vkInstance;

	uint32_t							m_gpuCount;
	std::vector<vk::PhysicalDevice>		m_gpu;

	uint32_t							m_queueFamilyCount;
	std::vector<vk::QueueFamilyProperties>	m_queueProps;
	
	uint32_t							m_graphicsQueueFamilyIndex;
	vk::Device							m_device;

	vk::CommandPool						m_commandPool;
	vk::CommandBuffer					m_commandBuffer;
};


