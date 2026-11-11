#include "IVulkan/vulkan_command_pool.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_device_queue.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "Volk/volk.h"
#include <exception>

namespace rhi {
	

	VulkanCommandPool::VulkanCommandPool(Device* inDevice, const CommandPoolDesc& inDesc) : CommandPool(inDevice,inDesc)
	{
		VulkanDeviceQueue& vulkanQueue = queue->API_VULKAN();
		VulkanDevice& vulkanDevice = device->API_VULKAN();

		VkCommandPoolCreateInfo createInfo{};
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex = vulkanQueue.GetFamilyIndex();
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		if (vkCreateCommandPool(vulkanDevice, &createInfo, nullptr, &commandPool) != VK_SUCCESS)
			throw std::exception("Couldn't create command pool");
	}

	std::vector<CommandBuffer*> VulkanCommandPool::AllocateCommandBuffers(uint32_t count, bool isPrimary)
	{
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
		std::vector<CommandBuffer*> commandBuffers;
		for (uint32_t i = 0; i < count; i++) 
		{
			std::unique_ptr<VulkanCommandBuffer> vulkanCommandBuffer = std::make_unique<VulkanCommandBuffer>();
			VulkanCommandBuffer* rawCommandBuffer = vulkanCommandBuffer.get();

			vulkanCommandBuffer->commandBuffer = vkBuffers[i];
			vulkanCommandBuffer->SetAllocationType(isPrimary);
			allocatedCommandBuffers.push_back(std::move(vulkanCommandBuffer));
			commandBuffers.push_back(rawCommandBuffer);
			
		}
		return commandBuffers;
	}
}