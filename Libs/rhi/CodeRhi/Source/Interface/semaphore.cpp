#include "Interface/semaphore.hpp"
#include <stdexcept>

namespace rhi
{
	Semaphore::Semaphore(Device* inDevice, const SemaphoreDesc& inDesc) :
		RHIResource(inDevice),
		desc(inDesc)
	{
	}

	bool Semaphore::IsBinary() const
	{
		return desc.Type == SemaphoreType::Binary;
	}

	VulkanSemaphore& Semaphore::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanSemaphore");
	}
}
