#pragma once
#include "Interface/command_pool.hpp"
#include "vulkan/vulkan.h"

namespace rhi {
	class VulkanCommandPool : public CommandPool {
	public :
		VulkanCommandPool(Device* inDevice, const CommandPoolDesc& inDesc);
		std::vector<CommandBuffer*> AllocateCommandBuffers(uint32_t inCount, bool inIsPrimary = true) override;

		VkCommandPool GetPool() const { return commandPool; }
		VulkanCommandPool& API_VULKAN() override { return *(this); }
	private :
		VkCommandPool commandPool = VK_NULL_HANDLE;
	};
}