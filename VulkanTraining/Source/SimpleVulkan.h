#pragma once

#include <vulkan/vulkan.hpp>

class SimpleVulkan
{
public:
	SimpleVulkan();
	~SimpleVulkan();

	void Init();
	void Cleanup();

private:
	vk::Instance m_vkInstance;
};


