#pragma once

#include "Interface/descriptor_pool.hpp"
#include "IVulkan/vulkan_descriptor_set.hpp"
#include "vulkan/vulkan.h"

namespace rhi
{
	class VulkanDevice;

	class VulkanDescriptorPool : public DescriptorPool
	{
	public :
		VulkanDescriptorPool(Device* inDevice, const DescriptorPoolDesc& inDesc);
		~VulkanDescriptorPool() override;

		void AllocateSets(DescriptorSet** outSets, SetLayout** inLayouts, uint32_t inSetCount, uint32_t* inVariableCounts = nullptr) override;
		void Reset() override;

		void CreatePoolForImGui();
		VkDescriptorPool GetPool() const;
		VulkanDescriptorPool& API_VULKAN() override;

	private :
		void AllocatePool();
		void FreePool();

	private :
		VulkanDevice& vulkanDevice;
		VkDescriptorPool pool = VK_NULL_HANDLE;
	};
}
