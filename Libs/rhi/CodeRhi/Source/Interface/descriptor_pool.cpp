#include "Interface/descriptor_pool.hpp"
#include "Interface/descriptor_set.hpp"
#include <stdexcept>

namespace rhi
{
	DescriptorPool::DescriptorPool(Device* inDevice, const DescriptorPoolDesc& inDesc) :
		RHIResource(inDevice),
		desc(inDesc)
	{
	}

	VulkanDescriptorPool& DescriptorPool::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanDescriptorPool");
	}
}
