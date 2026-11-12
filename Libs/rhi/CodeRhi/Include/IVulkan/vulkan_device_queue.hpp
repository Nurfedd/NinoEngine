#pragma once
#include "Interface/device_queue.hpp"
#include "IVulkan/vulkan_swapchain.hpp"
#include "Volk/volk.h"
#include <vector>
namespace rhi {
	class VulkanDevice;
	class VulkanFence;
	class VulkanSemaphore;
	
	class VulkanDeviceQueue : public DeviceQueue {
	public :
		VulkanDeviceQueue(VkDevice vulkanDevice, uint32_t familyIndex);
		
		void Submit(CommandBuffer** commandsBuffer, uint32_t commandBufferCount, SubmitInfo submitInfo, Fence* fence) override;
		void Submit(CommandBuffer** commandsBuffer, uint32_t commandBufferCount, Fence* fence) override;
		
		bool Present(PresentInfo presentInfo) override;
		VkQueue GetVkQueue() const { return queue; }
		uint32_t GetFamilyIndex() const { return familyIndex; }
		VulkanDeviceQueue& API_VULKAN() { return *(this); }
	private :
		std::vector<VkSemaphoreSubmitInfo> GetVulkanSubmitInfo(const SemaphoreSubmitInfo& submitInfo);
		VkQueue queue;
		uint32_t familyIndex;
	};
}