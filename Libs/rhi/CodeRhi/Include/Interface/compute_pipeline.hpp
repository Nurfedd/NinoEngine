#pragma once

#include "Interface/pipeline.hpp"
#include "Interface/push_constant_range.hpp"
#include <vector>

namespace rhi
{
	class Device;
	class SetLayout;
	class Shader;
	class VulkanComputePipeline;

	struct ComputePipelineCreateInfo
	{
	public :
		Shader* ComputeShader = nullptr;
		SetLayout** SetLayouts = nullptr;
		PushConstantRange* PushConstants = nullptr;
		uint32_t SetLayoutCount = 0;
		uint32_t PushConstantCount = 0;

		// Alias compatibilité
		Shader*& computeShader = ComputeShader;
		SetLayout**& setLayouts = SetLayouts;
		PushConstantRange*& pushConstants = PushConstants;
		uint32_t& setLayoutCount = SetLayoutCount;
		uint32_t& pushConstantCount = PushConstantCount;
	};

	class ComputePipeline : public Pipeline
	{
	public :
		ComputePipeline(Device* inDevice);
		~ComputePipeline() override = default;

		virtual void Build(const ComputePipelineCreateInfo& inCreateInfo) = 0;
		virtual VulkanComputePipeline& API_VULKAN();
	};
}
