#pragma once

#include "Interface/graphic_pipeline.hpp"
#include "IVulkan/vulkan_pipeline.hpp"

namespace rhi
{
	class VulkanDevice;

	class VulkanGraphicPipeline : public GraphicPipeline, public VulkanPipeline
	{
	public :
		VulkanGraphicPipeline(Device* inDevice);
		~VulkanGraphicPipeline() override;

		VkPipelineBindPoint GetBindPoint() const override
		{
			return VK_PIPELINE_BIND_POINT_GRAPHICS;
		}

		void Build(const GraphicVertexPipelineCreateInfo& inCreateInfo) override;
		void Build(const GraphicMeshPipelineCreateInfo& inCreateInfo) override;

		void PushConstant(CommandBuffer* inCommandBuffer, uint32_t inOffset, size_t inSize, const void* inData, ShaderStagesFlags inShaderStages) override;
		void BindDescriptorSet(CommandBuffer* inCommandBuffer, DescriptorSet** inSets, uint32_t inSetCount, uint32_t inFirstSet) override;
		void Bind(CommandBuffer* inCommandBuffer) override;

		std::vector<VkFormat> GetFormats();
		VkFormat GetDepthFormat();
		VulkanGraphicPipeline& API_VULKAN() override;

	private :
		void CreatePipelineInternal(const GraphicPipelineCreateInfo& inCreateInfo, const std::vector<VkPipelineShaderStageCreateInfo>& inShaderStages);
		void CreateShaderStages(const GraphicVertexPipelineCreateInfo& inCreateInfo, std::vector<VkPipelineShaderStageCreateInfo>& outShaderStages);
		void CreateShaderStages(const GraphicMeshPipelineCreateInfo& inCreateInfo, std::vector<VkPipelineShaderStageCreateInfo>& outShaderStages);

	private :
		VulkanDevice& vulkanDevice;
		std::vector<VkFormat> usedColorFormats;
		VkFormat usedDepthFormat = VK_FORMAT_UNDEFINED;
	};
}
