#include "Interface/sampler.hpp"
#include <stdexcept>

namespace rhi
{
	Sampler::Sampler(Device* inDevice, const SamplerDesc& inDesc) :
		RHIResource(inDevice),
		desc(inDesc)
	{
	}

	VulkanSampler& Sampler::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanSampler");
	}
}
