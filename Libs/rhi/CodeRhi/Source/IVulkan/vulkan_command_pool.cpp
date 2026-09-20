#include "IVulkan/vulkan_command_pool.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_device_queue.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "Volk/volk.h"
#include <exception>

namespace rhi {
	void VulkanCommandPool::Create(Device* device, DeviceQueue* _queue) {
		CommandPool::Create(device, _queue);
		VulkanDeviceQueue& vulkanQueue = queue->API_VULKAN();
		VulkanDevice& vulkanDevice = device->API_VULKAN();

		VkCommandPoolCreateInfo createInfo{};
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex = vulkanQueue.GetFamilyIndex();
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		if (vkCreateCommandPool(vulkanDevice, &createInfo, nullptr, &commandPool) != VK_SUCCESS)
			throw std::exception("Couldn't create command pool");
	}

	void VulkanCommandPool::AllocateCommandBufferVk(VulkanDevice& vulkanDevice, VulkanCommandBuffer* vulkanCommandsBuffer, uint32_t count, VkCommandBufferLevel level) {
		if (count == 0)
			return;
		
		std::vector<VkCommandBuffer> vkCommandBuffers(count);

		VkCommandBufferAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandBufferCount = count;
		allocateInfo.level = level;
		allocateInfo.commandPool = commandPool;

		bool isPrimary = (level == VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		if (vkAllocateCommandBuffers(vulkanDevice, &allocateInfo, vkCommandBuffers.data()) != VK_SUCCESS) {
			throw std::exception("Couldn't allocate command buffers");
		}
		for (uint32_t i = 0; i < count; i++) {
			vulkanCommandsBuffer[i].commandBuffer = vkCommandBuffers[i];
			vulkanCommandsBuffer[i].SetAllocationType(isPrimary);
		}
	}
	void VulkanCommandPool::AllocateCommandBuffers(Device* device,CommandBuffer** commandBuffers, uint32_t count,bool isPrimary) {
		if (count == 0)
			return;
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		VkCommandBufferLevel commandBufferLevel = isPrimary ? VK_COMMAND_BUFFER_LEVEL_PRIMARY: VK_COMMAND_BUFFER_LEVEL_SECONDARY;

		std::vector<VkCommandBuffer> vkBuffers(count);

		VkCommandBufferAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandBufferCount = count;
		allocateInfo.level = commandBufferLevel;
		allocateInfo.commandPool = commandPool;

		vkAllocateCommandBuffers(
			vulkanDevice,
			&allocateInfo,
			vkBuffers.data()
		);

		for (uint32_t i = 0; i < count; i++) {
			VulkanCommandBuffer& cmd = commandBuffers[i]->API_VULKAN();

			cmd.commandBuffer = vkBuffers[i];
			cmd.SetAllocationType(isPrimary);
		}
		
	}
	void VulkanCommandPool::Destroy(Device* device) {
		DestroyVk(device->API_VULKAN());
	}

	void VulkanCommandPool::DestroyVk(VulkanDevice& vulkanDevice) {
		if (commandPool)
			vkDestroyCommandPool(vulkanDevice, commandPool, nullptr);
	}
}