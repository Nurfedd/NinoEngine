#pragma once

#include "Interface/descriptor_pool.hpp"
#include "IVulkan/vulkan_descriptor_set.hpp"
#include "vulkan/vulkan.h"
#include <vector>
namespace rhi
{
	class VulkanDevice;

	class VulkanDescriptorPool : public DescriptorPool
	{
	public :
		VulkanDescriptorPool(Device* inDevice, const DescriptorPoolDesc& inDesc);
		~VulkanDescriptorPool() override;

		std::vector<DescriptorSet*> AllocateSets(SetLayout** inLayouts, uint32_t inSetCount, uint32_t* inVariableCounts) override;
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
