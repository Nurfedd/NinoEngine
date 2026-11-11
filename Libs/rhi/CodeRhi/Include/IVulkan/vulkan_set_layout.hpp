#pragma once

#include "Interface/set_layout.hpp"
#include "vulkan/vulkan.h"
#include <vector>

namespace rhi
{
	class VulkanDevice;

	class VulkanSetLayout : public SetLayout
	{
	public :
		VulkanSetLayout(Device* inDevice, const SetLayoutDesc& inDesc);
		~VulkanSetLayout() override;

		VkDescriptorSetLayout GetLayout() const;
		VulkanSetLayout& API_VULKAN() override;

	private :
		void AllocateLayout();
		void FreeLayout();

	private :
		VulkanDevice& vulkanDevice;
		VkDescriptorSetLayout setLayout = VK_NULL_HANDLE;
	};
}
