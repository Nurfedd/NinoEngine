#include "IVulkan/vulkan_compute_pipeline.hpp"
#include "IVulkan/vulkan_set_layout.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/vulkan_shader.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"
#include "Volk/volk.h"
#include <vector>
#include <stdexcept>

namespace rhi
{
	VulkanComputePipeline::VulkanComputePipeline(Device* inDevice) :
		ComputePipeline(inDevice),
		vulkanDevice(inDevice->API_VULKAN())
	{
	}

	VulkanComputePipeline::~VulkanComputePipeline()
	{
		VulkanPipeline::Destroy(vulkanDevice.GetVkDevice());
	}

	void VulkanComputePipeline::Build(const ComputePipelineCreateInfo& inCreateInfo)
	{
		std::vector<VkDescriptorSetLayout> layouts(inCreateInfo.SetLayoutCount);
		for (uint32_t i = 0; i < inCreateInfo.SetLayoutCount; i++)
		{
			layouts[i] = inCreateInfo.SetLayouts[i]->API_VULKAN().GetLayout();
		}

		std::vector<VkPushConstantRange> pushConstantRanges(inCreateInfo.PushConstantCount);
		for (uint32_t i = 0; i < inCreateInfo.PushConstantCount; i++)
		{
			const auto& pushConstantRange = inCreateInfo.PushConstants[i];
			pushConstantRanges[i].size = pushConstantRange.size;
			pushConstantRanges[i].offset = pushConstantRange.offset;
			pushConstantRanges[i].stageFlags = ToVulkanShaderStage(pushConstantRange.stages);
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = inCreateInfo.SetLayoutCount;
		pipelineLayoutInfo.pSetLayouts = layouts.data();
		pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();
		pipelineLayoutInfo.pushConstantRangeCount = inCreateInfo.PushConstantCount;

		if (vkCreatePipelineLayout(vulkanDevice.GetVkDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create compute pipeline layout");
		}

		VkPipelineShaderStageCreateInfo shaderInfo{};
		shaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderInfo.module = inCreateInfo.ComputeShader->API_VULKAN().GetShaderModule();
		shaderInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		shaderInfo.pName = "main";

		VkComputePipelineCreateInfo computeCreateInfo{};
		computeCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		computeCreateInfo.layout = pipelineLayout;
		computeCreateInfo.flags = 0;
		computeCreateInfo.basePipelineHandle = nullptr;
		computeCreateInfo.stage = shaderInfo;

		if (vkCreateComputePipelines(vulkanDevice.GetVkDevice(), nullptr, 1, &computeCreateInfo, nullptr, &pipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create compute pipeline");
		}
	}

	void VulkanComputePipeline::PushConstant(CommandBuffer* inCommandBuffer, uint32_t inOffset, size_t inSize, const void* inData, ShaderStagesFlags inShaderStages)
	{
		PushConstantVk(inCommandBuffer->API_VULKAN(), inOffset, inSize, const_cast<void*>(inData), ToVulkanShaderStage(inShaderStages));
	}

	void VulkanComputePipeline::BindDescriptorSet(CommandBuffer* inCommandBuffer, DescriptorSet** inSets, uint32_t inSetCount, uint32_t inFirstSet)
	{
		std::vector<VulkanDescriptorSet> vulkanSets(inSetCount, VulkanDescriptorSet(device));
		for (uint32_t i = 0; i < inSetCount; i++)
		{
			vulkanSets[i].SetVkSet(inSets[i]->API_VULKAN().GetVkSet());
		}
		BindDescriptorSetsVk(inCommandBuffer->API_VULKAN(), vulkanSets.data(), inSetCount, inFirstSet);
	}

	void VulkanComputePipeline::Bind(CommandBuffer* inCommandBuffer)
	{
		VulkanCommandBuffer& vulkanCommandBuffer = inCommandBuffer->API_VULKAN();
		VulkanPipeline::Bind(vulkanCommandBuffer);
	}

	VulkanComputePipeline& VulkanComputePipeline::API_VULKAN()
	{
		return *this;
	}
}
