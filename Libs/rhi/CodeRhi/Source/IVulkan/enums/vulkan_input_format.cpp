#include "IVulkan/enums/vulkan_input_format.hpp"

namespace rhi 
{
	VkFormat ToVulkanInputFormat(InputFormat inputFormat) 
	{
		return gInputFormatToVulkan[static_cast<uint32_t>(inputFormat)];
	}

	InputFormat FromVulkanInputFormat(VkFormat format) 
	{
		switch (format)
		{
		case VK_FORMAT_R32_SFLOAT: return InputFormat::Float1;
		case VK_FORMAT_R32G32_SFLOAT: return InputFormat::Float2;
		case VK_FORMAT_R32G32B32_SFLOAT: return InputFormat::Float3;
		case VK_FORMAT_R32G32B32A32_SFLOAT: return InputFormat::Float4;

		case VK_FORMAT_R32_SINT: return InputFormat::Int1;
		case VK_FORMAT_R32G32_SINT: return InputFormat::Int2;
		case VK_FORMAT_R32G32B32_SINT: return InputFormat::Int3;
		case VK_FORMAT_R32G32B32A32_SINT: return InputFormat::Int4;

		case VK_FORMAT_R32_UINT: return InputFormat::Uint32_1;
		case VK_FORMAT_R32G32_UINT: return InputFormat::Uint32_2;
		case VK_FORMAT_R32G32B32_UINT: return InputFormat::Uint32_3;
		case VK_FORMAT_R32G32B32A32_UINT: return InputFormat::Uint32_4;

		case VK_FORMAT_R64_UINT: return InputFormat::Uint64_1;
		case VK_FORMAT_R64G64_UINT: return InputFormat::Uint64_2;
		case VK_FORMAT_R64G64B64A64_UINT: return InputFormat::Uint64_4;
		default: return InputFormat::Float1;
		}
	}

	VkVertexInputRate ToVulkanInputRate(InputRate inputRate) 
	{
		return gInputRateToVulkan[static_cast<uint32_t>(inputRate)];
	}

	InputRate FromVulkanInputRate(VkVertexInputRate inputRate) 
	{
		switch (inputRate)
		{
		case VK_VERTEX_INPUT_RATE_VERTEX: return InputRate::Vertex;
		case VK_VERTEX_INPUT_RATE_INSTANCE: return InputRate::Instance;
		default: return InputRate::Vertex;
		}
	}

	VkDescriptorType ToVulkanDescriptorType(DescriptorType descriptorType) 
	{
		return gDescriptorTypeToVulkan[static_cast<uint32_t>(descriptorType)];
	}

	DescriptorType FromVulkanDescriptorType(VkDescriptorType vkDescriptorType) 
	{
		switch (vkDescriptorType)
		{
		case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return DescriptorType::SampledTexture;
		case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: return DescriptorType::UniformBuffer;
		case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: return DescriptorType::StorageBuffer;
		default: return DescriptorType::UniformBuffer;
		}
	}

	VkShaderStageFlags ToVulkanShaderStage(ShaderStagesFlags shaderStages)
	{
		VkShaderStageFlags vkFlags = 0;

		if (HasFlag(shaderStages, ShaderStagesFlags::Vertex))
			vkFlags |= VK_SHADER_STAGE_VERTEX_BIT;

		if (HasFlag(shaderStages, ShaderStagesFlags::Fragment))
			vkFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;

		if (HasFlag(shaderStages, ShaderStagesFlags::Geometry))
			vkFlags |= VK_SHADER_STAGE_GEOMETRY_BIT;

		if (HasFlag(shaderStages, ShaderStagesFlags::Compute))
			vkFlags |= VK_SHADER_STAGE_COMPUTE_BIT;

		return vkFlags;
	}

	ShaderStagesFlags FromVulkanShaderStages(VkShaderStageFlags vkFlags)
	{
		ShaderStagesFlags stages = ShaderStagesFlags::None;

		if (vkFlags & VK_SHADER_STAGE_VERTEX_BIT)
			stages |= ShaderStagesFlags::Vertex;

		if (vkFlags & VK_SHADER_STAGE_FRAGMENT_BIT)
			stages |= ShaderStagesFlags::Fragment;

		if (vkFlags & VK_SHADER_STAGE_GEOMETRY_BIT)
			stages |= ShaderStagesFlags::Geometry;

		if (vkFlags & VK_SHADER_STAGE_COMPUTE_BIT)
			stages |= ShaderStagesFlags::Compute;

		return stages;
	}

	VkIndexType ToVulkanIndexType(IndexType indexType) 
	{
		switch (indexType)
		{
		case IndexType::Uint8: return VK_INDEX_TYPE_UINT8;
		case IndexType::Uint16: return VK_INDEX_TYPE_UINT16;
		case IndexType::Uint32: return VK_INDEX_TYPE_UINT32;
		default: return VK_INDEX_TYPE_UINT32;
		}
	}
}