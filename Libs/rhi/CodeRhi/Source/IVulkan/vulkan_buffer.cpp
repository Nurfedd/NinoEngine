#include "IVulkan/vulkan_buffer.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/vulkan_barriers.hpp"
#include "IVulkan/enums/vulkan_buffer_usage.hpp"
#include "IVulkan/enums/vulkan_synchronization_stage.hpp"
#include "Volk/volk.h"
#include <cstring>
#include <stdexcept>

namespace rhi
{
	VulkanBuffer::VulkanBuffer(Device* inDevice, const BufferDesc& inDesc) :
		Buffer(inDevice, inDesc),
		vulkanDevice(inDevice->API_VULKAN())
	{
		AllocateBuffer();
	}

	VulkanBuffer::~VulkanBuffer()
	{
		FreeBuffer();
	}

	void VulkanBuffer::AllocateBuffer()
	{
		bufferSize = static_cast<VkDeviceSize>(desc.Size);
		vkBufferUsage = ToVulkanBufferUsage(desc.Usage);

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = bufferSize;
		bufferInfo.usage = vkBufferUsage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo allocInfo{};
		if (desc.IsCpuAccessible)
		{
			allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT;
			allocInfo.requiredFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
			allocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_HOST;
			vkRequiredFlags = allocInfo.requiredFlags;
		}
		else
		{
			allocInfo.flags = 0;
			allocInfo.requiredFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			allocInfo.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
			vkRequiredFlags = allocInfo.requiredFlags;
		}

		VmaAllocationInfo resultAllocInfo{};
		VkResult res = vmaCreateBuffer(
			vulkanDevice.GetAllocator(),
			&bufferInfo,
			&allocInfo,
			&buffer,
			&allocation,
			&resultAllocInfo
		);

		if (res != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate Vulkan buffer via VMA");
		}

		if (desc.IsCpuAccessible)
		{
			mappedPtr = resultAllocInfo.pMappedData;
		}
	}

	void VulkanBuffer::FreeBuffer()
	{
		if (buffer != VK_NULL_HANDLE || allocation != nullptr)
		{
			if (mappedPtr != nullptr && !desc.IsCpuAccessible)
			{
				vmaUnmapMemory(vulkanDevice.GetAllocator(), allocation);
				mappedPtr = nullptr;
			}
			vulkanDevice.DestroyBuffer(buffer, allocation);
			buffer = VK_NULL_HANDLE;
			allocation = nullptr;
			mappedPtr = nullptr;
		}
	}

	void VulkanBuffer::Upload(CommandBuffer* commandBuffer,const void* inData, size_t inSize, size_t inOffset)
	{
		if (inData == nullptr || inSize == 0)
		{
			return;
		}

		if (desc.IsCpuAccessible)
		{
			void* mapped = GetMappedData();
			if (mapped == nullptr)
			{
				throw std::runtime_error("Cannot map CPU buffer memory");
			}
			std::memcpy(static_cast<char*>(mapped) + inOffset, inData, inSize);
		}
		else
		{
			BufferDesc stagingDesc(BufferUsage::TransferSrc,inSize,true);
			

			VulkanBuffer stagingBuffer(device, stagingDesc);
			stagingBuffer.Upload(commandBuffer,inData, inSize, 0);

			CopyFromBuffer(commandBuffer->API_VULKAN(), stagingBuffer,inSize,0,inOffset);
		}
	}

	void VulkanBuffer::Read(void* outData, size_t inSize, size_t inOffset)
	{
		if (outData == nullptr || inSize == 0)
		{
			return;
		}

		if (desc.IsCpuAccessible)
		{
			void* mapped = GetMappedData();
			if (mapped == nullptr)
			{
				throw std::runtime_error("Cannot map CPU buffer for reading");
			}
			std::memcpy(outData, static_cast<const char*>(mapped) + inOffset, inSize);
		}
		else
		{
			throw std::runtime_error("Direct GPU buffer read is not supported without a staging copy");
		}
	}

	void* VulkanBuffer::GetMappedData()
	{
		if (!desc.IsCpuAccessible)
		{
			return nullptr;
		}

		if (mappedPtr == nullptr && allocation != nullptr)
		{
			vmaMapMemory(vulkanDevice.GetAllocator(), allocation, &mappedPtr);
		}
		return mappedPtr;
	}

	void VulkanBuffer::Transition(CommandBuffer* inCommandBuffer, BufferState inDstState)
	{
		if (inCommandBuffer == nullptr)
		{
			return;
		}
		TransitionBufferVk(inCommandBuffer->API_VULKAN(), ToVulkanBufferState(inDstState));
	}

	void VulkanBuffer::TransitionBufferVk(VulkanCommandBuffer& inCommandBuffer, VulkanBufferState inState)
	{
		VulkanBufferBarrier bufferBarrier;
		bufferBarrier.SetStages(
			currentBufferState.pipelineStage,
			currentBufferState.accessFlags,
			inState.pipelineStage,
			inState.accessFlags
		);
		bufferBarrier.buffer = buffer;
		bufferBarrier.offset = 0;
		bufferBarrier.size = VK_WHOLE_SIZE;

		BarriersDependencyVk dep{};
		dep.bufferBarriers = &bufferBarrier;
		dep.bufferBarrierCount = 1;
		inCommandBuffer.PipelineBarrierVk(dep);

		currentBufferState = inState;
	}

	void VulkanBuffer::CopyFromBuffer(VulkanCommandBuffer& inCommandBuffer, const VulkanBuffer& inSrcBuffer, size_t inCopySize, size_t inSrcOffset, size_t inDstOffset)
	{
		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = static_cast<VkDeviceSize>(inSrcOffset);
		copyRegion.dstOffset = static_cast<VkDeviceSize>(inDstOffset);
		copyRegion.size = static_cast<VkDeviceSize>(inCopySize);
		vkCmdCopyBuffer(inCommandBuffer.commandBuffer, inSrcBuffer.buffer, buffer, 1, &copyRegion);
	}

	uint64_t VulkanBuffer::GetDeviceAddress()
	{
		VkBufferDeviceAddressInfo info{};
		info.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
		info.buffer = buffer;
		return vkGetBufferDeviceAddress(vulkanDevice.GetVkDevice(), &info);
	}

	VkBuffer VulkanBuffer::GetBuffer() const
	{
		return buffer;
	}

	VkDeviceSize VulkanBuffer::GetVkBufferSize() const
	{
		return bufferSize;
	}

	VulkanBuffer& VulkanBuffer::API_VULKAN()
	{
		return *this;
	}
}
