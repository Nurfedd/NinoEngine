#include "Interface/physical_device.hpp"
#include <stdexcept>

namespace rhi
{
	VulkanPhysicalDevice& PhysicalDevice::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanPhysicalDevice");
	}
}

