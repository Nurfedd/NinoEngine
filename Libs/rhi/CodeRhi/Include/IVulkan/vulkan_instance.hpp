#pragma once
#include "Interface/instance.hpp"
#include "IVulkan/vulkan_physical_device.hpp"
#include "vulkan/vulkan.h"
#include <memory>

namespace rhi {
	class VulkanInstance : public Instance {
	public :
		VulkanInstance(const char* applicationName);
		~VulkanInstance() override;

		virtual std::unique_ptr<Device> CreateDevice(PhysicalDevice* inPhysicalDevice, Surface* inSurface) override;

		VulkanInstance& API_VULKAN() override { return *(this); }
		VkInstance GetVkInstance() const { return vkInstance; }
		uint32_t GetApiVersion() {return VK_API_VERSION_1_4 ;}
	private :
		VkInstance vkInstance = VK_NULL_HANDLE;
		void LoadPhysicalDevices() override;
	};
}