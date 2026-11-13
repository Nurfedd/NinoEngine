#include "Interface/descriptor_set.hpp"
#include <stdexcept>

namespace rhi
{
	DescriptorSet::DescriptorSet(Device* inDevice) : device(inDevice) {}

	VulkanDescriptorSet& DescriptorSet::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanDescriptorSet");
	}
}
