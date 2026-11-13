#include "IVulkan/vulkan_graphic_pipeline.hpp"
#include "IVulkan/vulkan_vertex_layout.hpp"
#include "IVulkan/vulkan_shader.hpp"
#include "IVulkan/vulkan_set_layout.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/enums/vulkan_rasterizer_state.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"
#include "IVulkan/enums/vulkan_format.hpp"
#include "Volk/volk.h"
#include <vector>
#include <stdexcept>

namespace rhi
{
	VulkanGraphicPipeline::VulkanGraphicPipeline(Device* inDevice) :
		GraphicPipeline(inDevice),
		vulkanDevice(inDevice->API_VULKAN())
	{
	}

	VulkanGraphicPipeline::~VulkanGraphicPipeline()
	{
		VulkanPipeline::Destroy(vulkanDevice.GetVkDevice());
	}

	void VulkanGraphicPipeline::Build(const GraphicVertexPipelineCreateInfo& inCreateInfo)
	{
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		CreateShaderStages(inCreateInfo, shaderStages);
		CreatePipelineInternal(inCreateInfo, shaderStages);
	}

	void VulkanGraphicPipeline::Build(const GraphicMeshPipelineCreateInfo& inCreateInfo)
	{
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		CreateShaderStages(inCreateInfo, shaderStages);
		CreatePipelineInternal(inCreateInfo, shaderStages);
	}

	void VulkanGraphicPipeline::CreatePipelineInternal(const GraphicPipelineCreateInfo& inCreateInfo, const std::vector<VkPipelineShaderStageCreateInfo>& inShaderStages)
	{
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<uint32_t>(inShaderStages.size());
		pipelineInfo.pStages = inShaderStages.data();

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		std::vector<VkVertexInputBindingDescription> bindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

		for (uint32_t i = 0; i < inCreateInfo.VertexLayoutCount; i++)
		{
			std::vector<VkVertexInputAttributeDescription> inputDescriptions;
			VkVertexInputBindingDescription bindingDescription{};
			VertexLayout& vertexLayout = inCreateInfo.VertexLayouts[i];

			GetVulkanVertexLayoutObjects(vertexLayout, inputDescriptions, bindingDescription);
			
			bindingDescriptions.push_back(bindingDescription);
			for (const auto& desc : inputDescriptions)
			{
				attributeDescriptions.push_back(desc);
			}
		}

		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = ToVulkanPolygonMode(inCreateInfo.RasterInfo.Mode);
		rasterizer.lineWidth = 1.0f;
		
		rasterizer.cullMode = ToVulkanCullModeFlags(inCreateInfo.RasterInfo.CullFlags);
		rasterizer.frontFace = ToVulkanFrontFace(inCreateInfo.RasterInfo.Face);
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments(inCreateInfo.RasterInfo.AttachmentCount);
		for (uint32_t i = 0; i < inCreateInfo.RasterInfo.AttachmentCount; i++)
		{
			colorBlendAttachments[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachments[i].blendEnable = inCreateInfo.RasterInfo.ColorAttachments[i].EnableBlending ? VK_TRUE : VK_FALSE;
			colorBlendAttachments[i].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			colorBlendAttachments[i].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			colorBlendAttachments[i].colorBlendOp = VK_BLEND_OP_ADD;
			colorBlendAttachments[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			colorBlendAttachments[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorBlendAttachments[i].alphaBlendOp = VK_BLEND_OP_ADD;
		}

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = static_cast<uint32_t>(colorBlendAttachments.size());
		colorBlending.pAttachments = colorBlendAttachments.data();
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = inCreateInfo.RasterInfo.DepthTestEnable;
		depthStencil.depthWriteEnable = inCreateInfo.RasterInfo.DepthWrite;
		depthStencil.depthCompareOp = ToVulkanCompareOp(inCreateInfo.RasterInfo.DepthOp);
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.stencilTestEnable = VK_FALSE;

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
		pipelineLayoutInfo.pushConstantRangeCount = inCreateInfo.PushConstantCount;
		pipelineLayoutInfo.pPushConstantRanges = pushConstantRanges.data();

		if (vkCreatePipelineLayout(vulkanDevice.GetVkDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create graphic pipeline layout");
		}

		std::vector<VkFormat> vkFormats(inCreateInfo.RasterInfo.AttachmentCount);
		for (uint32_t i = 0; i < vkFormats.size(); i++)
		{
			vkFormats[i] = ToVulkanFormat(inCreateInfo.RasterInfo.ColorAttachments[i].TextureFormat);
		}

		VkPipelineRenderingCreateInfoKHR renderingCreateInfo{};
		renderingCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
		renderingCreateInfo.colorAttachmentCount = static_cast<uint32_t>(vkFormats.size());
		renderingCreateInfo.pColorAttachmentFormats = vkFormats.data();
		renderingCreateInfo.depthAttachmentFormat = ToVulkanFormat(inCreateInfo.RasterInfo.DepthTextureFormat);

		pipelineInfo.pNext = &renderingCreateInfo;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = VK_NULL_HANDLE;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;

		if (vkCreateGraphicsPipelines(vulkanDevice.GetVkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create graphics pipeline");
		}

		usedColorFormats = vkFormats;
		usedDepthFormat = renderingCreateInfo.depthAttachmentFormat;
	}

	void VulkanGraphicPipeline::CreateShaderStages(const GraphicVertexPipelineCreateInfo& inCreateInfo, std::vector<VkPipelineShaderStageCreateInfo>& outShaderStages)
	{
		if (inCreateInfo.VertexShader != nullptr)
		{
			VkPipelineShaderStageCreateInfo stageInfo{};
			stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			stageInfo.module = inCreateInfo.VertexShader->API_VULKAN().GetShaderModule();
			stageInfo.pName = "main";
			outShaderStages.push_back(stageInfo);
		}
		if (inCreateInfo.FragmentShader != nullptr)
		{
			VkPipelineShaderStageCreateInfo stageInfo{};
			stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			stageInfo.module = inCreateInfo.FragmentShader->API_VULKAN().GetShaderModule();
			stageInfo.pName = "main";
			outShaderStages.push_back(stageInfo);
		}
		if (inCreateInfo.GeometryShader != nullptr)
		{
			VkPipelineShaderStageCreateInfo stageInfo{};
			stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
			stageInfo.module = inCreateInfo.GeometryShader->API_VULKAN().GetShaderModule();
			stageInfo.pName = "main";
			outShaderStages.push_back(stageInfo);
		}
	}

	void VulkanGraphicPipeline::CreateShaderStages(const GraphicMeshPipelineCreateInfo& inCreateInfo, std::vector<VkPipelineShaderStageCreateInfo>& outShaderStages)
	{
		if (inCreateInfo.FragmentShader != nullptr)
		{
			VkPipelineShaderStageCreateInfo stageInfo{};
			stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			stageInfo.module = inCreateInfo.FragmentShader->API_VULKAN().GetShaderModule();
			stageInfo.pName = "main";
			outShaderStages.push_back(stageInfo);
		}
		if (inCreateInfo.TaskShader != nullptr)
		{
			VkPipelineShaderStageCreateInfo stageInfo{};
			stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.stage = VK_SHADER_STAGE_TASK_BIT_EXT;
			stageInfo.module = inCreateInfo.TaskShader->API_VULKAN().GetShaderModule();
			stageInfo.pName = "main";
			outShaderStages.push_back(stageInfo);
		}
		if (inCreateInfo.MeshShader != nullptr)
		{
			VkPipelineShaderStageCreateInfo stageInfo{};
			stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.stage = VK_SHADER_STAGE_MESH_BIT_EXT;
			stageInfo.module = inCreateInfo.MeshShader->API_VULKAN().GetShaderModule();
			stageInfo.pName = "main";
			outShaderStages.push_back(stageInfo);
		}
	}

	void VulkanGraphicPipeline::PushConstant(CommandBuffer* inCommandBuffer, uint32_t inOffset, size_t inSize, const void* inData, ShaderStagesFlags inShaderStages)
	{
		PushConstantVk(inCommandBuffer->API_VULKAN(), inOffset, inSize, const_cast<void*>(inData), ToVulkanShaderStage(inShaderStages));
	}

	void VulkanGraphicPipeline::BindDescriptorSet(CommandBuffer* inCommandBuffer, DescriptorSet** inSets, uint32_t inSetCount, uint32_t inFirstSet)
	{
		std::vector<VulkanDescriptorSet> vulkanSets(inSetCount, VulkanDescriptorSet(device));
		for (uint32_t i = 0; i < inSetCount; i++)
		{
			vulkanSets[i].SetVkSet(inSets[i]->API_VULKAN().GetVkSet());
		}
		BindDescriptorSetsVk(inCommandBuffer->API_VULKAN(), vulkanSets.data(), inSetCount, inFirstSet);
	}

	void VulkanGraphicPipeline::Bind(CommandBuffer* inCommandBuffer)
	{
		VulkanCommandBuffer& vulkanCommandBuffer = inCommandBuffer->API_VULKAN();
		VulkanPipeline::Bind(vulkanCommandBuffer);
	}

	std::vector<VkFormat> VulkanGraphicPipeline::GetFormats()
	{
		return usedColorFormats;
	}

	VkFormat VulkanGraphicPipeline::GetDepthFormat()
	{
		return usedDepthFormat;
	}

	VulkanGraphicPipeline& VulkanGraphicPipeline::API_VULKAN()
	{
		return *this;
	}
}
