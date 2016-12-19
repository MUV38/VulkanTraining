#pragma once

#define VK_USE_PLATFORM_WIN32_KHR

#include <Windows.h>
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
};


