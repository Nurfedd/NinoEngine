#include "IVulkan/vulkan_interface.hpp"
#include "IVulkan/vulkan_instance.hpp"
#include "IVulkan/vulkan_surface.hpp"


namespace rhi {
	std::shared_ptr<Instance> VulkanInterface::CreateInstance(const char* applicationName)
	{
		return std::make_shared<VulkanInstance>(applicationName);
	}
	std::unique_ptr<Surface> VulkanInterface::CreateSurface(std::shared_ptr<Instance> instance, GLFWwindow* window)
	{
		return std::make_unique<VulkanSurface>(instance, window);
	}
}