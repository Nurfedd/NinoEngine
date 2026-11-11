#include "IVulkan/vulkan_fence.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "Volk/volk.h"
#include <stdexcept>

namespace rhi
{
	VulkanFence::VulkanFence(Device* inDevice, const FenceDesc& inDesc) :
		Fence(inDevice, inDesc),
		vulkanDevice(inDevice->API_VULKAN())
	{
		AllocateFence();
	}

	VulkanFence::~VulkanFence()
	{
		FreeFence();
	}

	void VulkanFence::AllocateFence()
	{
		VkFenceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		if (desc.Signaled)
		{
			createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		}

		if (vkCreateFence(vulkanDevice.GetVkDevice(), &createInfo, nullptr, &fence) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan fence");
		}
	}

	void VulkanFence::FreeFence()
	{
		if (fence != VK_NULL_HANDLE)
		{
			vkDestroyFence(vulkanDevice.GetVkDevice(), fence, nullptr);
			fence = VK_NULL_HANDLE;
		}
	}

	bool VulkanFence::IsReady()
	{
		VkResult res = vkGetFenceStatus(vulkanDevice.GetVkDevice(), fence);
		return res == VK_SUCCESS;
	}

	void VulkanFence::Reset()
	{
		vkResetFences(vulkanDevice.GetVkDevice(), 1, &fence);
	}

	void VulkanFence::Wait(uint64_t inTimeout)
	{
		vkWaitForFences(vulkanDevice.GetVkDevice(), 1, &fence, VK_TRUE, inTimeout);
	}

	VkFence VulkanFence::GetFence() const
	{
		return fence;
	}

	VulkanFence& VulkanFence::API_VULKAN()
	{
		return *this;
	}
}
