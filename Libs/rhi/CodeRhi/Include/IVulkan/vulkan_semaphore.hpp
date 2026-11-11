#pragma once

#include "Interface/semaphore.hpp"
#include "vulkan/vulkan.h"

namespace rhi
{
	class VulkanDevice;

	class VulkanSemaphore : public Semaphore
	{
	public :
		VulkanSemaphore(Device* inDevice, const SemaphoreDesc& inDesc);
		~VulkanSemaphore() override;

		uint64_t GetTimelineValue() override;
		VkSemaphore GetSemaphore() const;
		VulkanSemaphore& API_VULKAN() override;

	private :
		void AllocateSemaphore();
		void FreeSemaphore();

	private :
		VulkanDevice& vulkanDevice;
		VkSemaphore semaphore = VK_NULL_HANDLE;
	};
}
