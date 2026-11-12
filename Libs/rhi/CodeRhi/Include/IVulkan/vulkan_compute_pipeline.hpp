#pragma once

#include "Interface/compute_pipeline.hpp"
#include "IVulkan/vulkan_pipeline.hpp"

namespace rhi
{
	class VulkanDevice;

	class VulkanComputePipeline : public ComputePipeline, public VulkanPipeline
	{
	public :
		VulkanComputePipeline(Device* inDevice);
		~VulkanComputePipeline() override;

		VkPipelineBindPoint GetBindPoint() const override
		{
			return VK_PIPELINE_BIND_POINT_COMPUTE;
		}

		void Build(const ComputePipelineCreateInfo& inCreateInfo) override;
		void PushConstant(CommandBuffer* inCommandBuffer, uint32_t inOffset, size_t inSize, const void* inData, ShaderStagesFlags inShaderStages) override;
		void BindDescriptorSet(CommandBuffer* inCommandBuffer, DescriptorSet** inSets, uint32_t inSetCount, uint32_t inFirstSet) override;
		void Bind(CommandBuffer* inCommandBuffer) override;

		VulkanComputePipeline& API_VULKAN() override;

	private :
		VulkanDevice& vulkanDevice;
	};
}
