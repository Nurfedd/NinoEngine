#include "Interface/fence.hpp"
#include <stdexcept>

namespace rhi
{
	Fence::Fence(Device* inDevice, const FenceDesc& inDesc) :
		RHIResource(inDevice),
		desc(inDesc)
	{
	}

	VulkanFence& Fence::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanFence");
	}
}
