#pragma once
#include "Interface/swapchain.hpp"
#include "vulkan/vulkan.h"


namespace rhi {
	class VulkanDevice;
	class VulkanSwapchain : public Swapchain {
	public :
		VulkanSwapchain(Device* inDevice, SwapchainCreateInfo swapchainCreateInfo);
		~VulkanSwapchain() override;
		SwapchainAcquireResult AcquireImage(Fence* fence, Semaphore* semaphore) override;
		void PreparePresentImage(CommandBuffer* commandBuffer) override;
		void Recreate() override;
		
		VkSwapchainKHR GetSwapchain() { return swapchain; }
		VulkanSwapchain& API_VULKAN() override { return *(this); }
		uint32_t GetWidth() override { return swapchainExtent.width; }
		uint32_t GetHeight() override { return swapchainExtent.height; }
	private :
		
		VkSwapchainKHR swapchain = VK_NULL_HANDLE;
		VkExtent2D swapchainExtent;
		void DestroyImages() override;
		void CreateImages() override;
	};
}