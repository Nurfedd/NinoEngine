#include "IVulkan/enums/vulkan_buffer_usage.hpp"
#include "vulkan/vulkan.h"

namespace rhi 
{
	VkBufferUsageFlags ToVulkanBufferUsage(BufferUsage bufferUsage) 
	{
		VkBufferUsageFlags flags = 0;
		if (HasFlag(bufferUsage, BufferUsage::UniformBuffer))
			flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		if (HasFlag(bufferUsage, BufferUsage::StorageBuffer))
			flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		if (HasFlag(bufferUsage, BufferUsage::IndexBuffer))
			flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		if (HasFlag(bufferUsage, BufferUsage::VertexBuffer))
			flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		if (HasFlag(bufferUsage, BufferUsage::IndirectCommandBuffer))
			flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
		if (HasFlag(bufferUsage, BufferUsage::TransferSrc))
			flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		if (HasFlag(bufferUsage, BufferUsage::TransferDst))
			flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		return flags;
	}

	BufferUsage FromVulkanBufferUsage(VkBufferUsageFlags vkBufferUsage) 
	{
		BufferUsage flags = static_cast<BufferUsage>(0);

		if (vkBufferUsage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
			flags |= BufferUsage::UniformBuffer;
		if (vkBufferUsage & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
			flags |= BufferUsage::StorageBuffer;
		if (vkBufferUsage & VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
			flags |= BufferUsage::IndexBuffer;
		if (vkBufferUsage & VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
			flags |= BufferUsage::VertexBuffer;
		if (vkBufferUsage & VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT)
			flags |= BufferUsage::IndirectCommandBuffer;
		if (vkBufferUsage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
			flags |= BufferUsage::TransferSrc;
		if (vkBufferUsage & VK_BUFFER_USAGE_TRANSFER_DST_BIT)
			flags |= BufferUsage::TransferDst;

		return flags;
	}

	VulkanBufferState ToVulkanBufferState(BufferState bufferState)
	{
		switch (bufferState)
		{
		case BufferState::ComputeWrite:
			return {
				VK_ACCESS_2_SHADER_WRITE_BIT,
				VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
			};

		case BufferState::ComputeRead:
			return {
				VK_ACCESS_2_SHADER_SAMPLED_READ_BIT | VK_ACCESS_2_SHADER_STORAGE_READ_BIT,
				VK_PIPELINE_STAGE_2_COMPUTE_SHADER_BIT
			};

		case BufferState::VertexBuffer:
			return {
				VK_ACCESS_2_VERTEX_ATTRIBUTE_READ_BIT,
				VK_PIPELINE_STAGE_2_VERTEX_INPUT_BIT
			};

		case BufferState::IndexBuffer:
			return {
				VK_ACCESS_2_INDEX_READ_BIT,
				VK_PIPELINE_STAGE_2_INDEX_INPUT_BIT
			};

		case BufferState::IndirectDraw:
			return {
				VK_ACCESS_2_INDIRECT_COMMAND_READ_BIT,
				VK_PIPELINE_STAGE_2_DRAW_INDIRECT_BIT
			};

		case BufferState::TransferSrc:
			return {
				VK_ACCESS_2_TRANSFER_READ_BIT,
				VK_PIPELINE_STAGE_2_TRANSFER_BIT
			};

		case BufferState::TransferDst:
			return {
				VK_ACCESS_2_TRANSFER_WRITE_BIT,
				VK_PIPELINE_STAGE_2_TRANSFER_BIT
			};

		case BufferState::Undefined:
		default:
			return {
				0,
				VK_PIPELINE_STAGE_2_NONE
			};
		}
	}
}