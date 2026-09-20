#pragma once
#include "Interface/command_pool.hpp"
#include "vulkan/vulkan.h"

namespace rhi {
	class VulkanCommandPool : public CommandPool {
	public :
		void Create(Device* device, DeviceQueue* _queue) override;
		void AllocateCommandBufferVk(VulkanDevice& vulkanDevice, VulkanCommandBuffer* vulkanCommandsBuffer, uint32_t count, VkCommandBufferLevel level);
		void AllocateCommandBuffers(Device* device,CommandBuffer** commandBuffer, uint32_t count,bool isPrimary = true) override;
		void Destroy(Device* device) override;
		void DestroyVk(VulkanDevice& vulkanDevice);
		VkCommandPool GetPool() const { return commandPool; }
		VulkanCommandPool& API_VULKAN() override { return *(this); }
	private :
		VkCommandPool commandPool = VK_NULL_HANDLE;
	};
}