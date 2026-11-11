#include "IVulkan/vulkan_semaphore.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "Volk/volk.h"
#include <stdexcept>

namespace rhi
{
	VulkanSemaphore::VulkanSemaphore(Device* inDevice, const SemaphoreDesc& inDesc) :
		Semaphore(inDevice, inDesc),
		vulkanDevice(inDevice->API_VULKAN())
	{
		AllocateSemaphore();
	}

	VulkanSemaphore::~VulkanSemaphore()
	{
		FreeSemaphore();
	}

	void VulkanSemaphore::AllocateSemaphore()
	{
		VkSemaphoreCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkSemaphoreTypeCreateInfo typeInfo{};
		typeInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
		if (desc.Type == SemaphoreType::Timeline)
		{
			typeInfo.semaphoreType = VK_SEMAPHORE_TYPE_TIMELINE;
			typeInfo.initialValue = desc.InitialValue;
			createInfo.pNext = &typeInfo;
		}

		if (vkCreateSemaphore(vulkanDevice.GetVkDevice(), &createInfo, nullptr, &semaphore) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan semaphore");
		}
	}

	void VulkanSemaphore::FreeSemaphore()
	{
		if (semaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(vulkanDevice.GetVkDevice(), semaphore, nullptr);
			semaphore = VK_NULL_HANDLE;
		}
	}

	uint64_t VulkanSemaphore::GetTimelineValue()
	{
		if (desc.Type != SemaphoreType::Timeline)
		{
			return 0;
		}

		uint64_t value = 0;
		vkGetSemaphoreCounterValue(vulkanDevice.GetVkDevice(), semaphore, &value);
		return value;
	}

	VkSemaphore VulkanSemaphore::GetSemaphore() const
	{
		return semaphore;
	}

	VulkanSemaphore& VulkanSemaphore::API_VULKAN()
	{
		return *this;
	}
}
