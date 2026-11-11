#pragma once

#include "vulkan/vulkan.h"
#include "IVulkan/vk_mem_alloc.h"
#include "IVulkan/enums/vulkan_buffer_usage.hpp"
#include "IVulkan/enums/vulkan_synchronization_stage.hpp"
#include "Interface/buffer.hpp"
#include <string>

namespace rhi
{
	class VulkanDevice;
	class VulkanCommandBuffer;
	class VulkanUploadContext;
	class VulkanBuffer : public Buffer
	{
	public :
		VulkanBuffer(Device* inDevice, const BufferDesc& inDesc);
		~VulkanBuffer() override;

		void Upload(CommandBuffer* commandBuffer,const void* inData, size_t inSize, size_t inOffset = 0) override;
		void Read(void* outData, size_t inSize, size_t inOffset = 0) override;
		void* GetMappedData() override;
		void Transition(CommandBuffer* inCommandBuffer, BufferState inDstState) override;
		uint64_t GetDeviceAddress() override;

		void TransitionBufferVk(VulkanCommandBuffer& inCommandBuffer, VulkanBufferState inState);
		void CopyFromBuffer(VulkanCommandBuffer& inCommandBuffer, const VulkanBuffer& inSrcBuffer, size_t inCopySize, size_t inSrcOffset = 0, size_t inDstOffset = 0);

		VkBuffer GetBuffer() const;
		VkDeviceSize GetVkBufferSize() const;
		VulkanBuffer& API_VULKAN() override;

	private :
		void AllocateBuffer();
		void FreeBuffer();

	private :
		VulkanDevice& vulkanDevice;
		VkBuffer buffer = VK_NULL_HANDLE;
		VmaAllocation allocation = nullptr;
		void* mappedPtr = nullptr;
		VkDeviceSize bufferSize = 0;
		VkBufferUsageFlags vkBufferUsage = 0;
		VkMemoryPropertyFlags vkFlags = 0;
		VkMemoryPropertyFlags vkRequiredFlags = 0;
		VulkanBufferState currentBufferState{};
	};
}
