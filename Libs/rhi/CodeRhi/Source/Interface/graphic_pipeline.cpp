#include "Interface/graphic_pipeline.hpp"
#include <stdexcept>

namespace rhi
{
	GraphicPipeline::GraphicPipeline(Device* inDevice) :
		Pipeline(inDevice)
	{
	}

	VulkanGraphicPipeline& GraphicPipeline::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanGraphicPipeline");
	}
}
