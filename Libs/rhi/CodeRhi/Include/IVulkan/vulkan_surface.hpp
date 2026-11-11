#pragma once
#include "Interface/surface.hpp"
#include "Interface/instance.hpp"
#include "vulkan/vulkan.h"

namespace rhi {
	
	class VulkanSurface : public Surface {
	public :
		VulkanSurface(std::shared_ptr<Instance> inInstance, GLFWwindow* window);
		~VulkanSurface() override;

		void LoadFormats(Device* device) override;
		void GetCapabilities(VkPhysicalDevice physicalDevice,VkSurfaceCapabilitiesKHR* capabilities);

		VulkanSurface& API_VULKAN() override { return *(this); };
		VkSurfaceKHR GetSurface() const { return surface; }
	protected :
		void LoadPresentMode(Device* device);
		void LoadSurfaceFormat(Device* device);
		VkSurfaceKHR surface;
	};
}