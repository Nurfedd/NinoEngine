#include "Interface/command_pool.hpp"
#include <stdexcept>

namespace rhi
{
	CommandPool::CommandPool(Device* inDevice, const CommandPoolDesc& inDesc) :
		RHIResource(inDevice),
		desc(inDesc),
		queue(inDesc.Queue)
	{
	}

	VulkanCommandPool& CommandPool::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanCommandPool");
	}
}

