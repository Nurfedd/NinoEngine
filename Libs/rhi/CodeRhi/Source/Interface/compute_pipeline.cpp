#include "Interface/compute_pipeline.hpp"
#include <stdexcept>

namespace rhi
{
	ComputePipeline::ComputePipeline(Device* inDevice) :
		Pipeline(inDevice)
	{
	}

	VulkanComputePipeline& ComputePipeline::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanComputePipeline");
	}
}
