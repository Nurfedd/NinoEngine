#pragma once

#include "Interface/fence.hpp"
#include "vulkan/vulkan.h"

namespace rhi
{
	class VulkanDevice;

	class VulkanFence : public Fence
	{
	public :
		VulkanFence(Device* inDevice, const FenceDesc& inDesc);
		~VulkanFence() override;

		bool IsReady() override;
		void Reset() override;
		void Wait(uint64_t inTimeout = UINT64_MAX) override;

		VkFence GetFence() const;
		VulkanFence& API_VULKAN() override;

	private :
		void AllocateFence();
		void FreeFence();

	private :
		VulkanDevice& vulkanDevice;
		VkFence fence = VK_NULL_HANDLE;
	};
}
