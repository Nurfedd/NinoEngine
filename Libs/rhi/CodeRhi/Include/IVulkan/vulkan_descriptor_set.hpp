#pragma once

#include "Interface/descriptor_set.hpp"
#include "vulkan/vulkan.h"

namespace rhi
{
	class VulkanDevice;

	class VulkanDescriptorSet : public DescriptorSet
	{
	public :
		VulkanDescriptorSet(Device* inDevice);
		~VulkanDescriptorSet() override = default;

		void UpdateBuffer(uint32_t inBinding, BufferUpdateInfo inBufferUpdateInfo, DescriptorType inDescriptorType) override;
		void UpdateTexture(uint32_t inBinding, Texture* inTexture, Sampler* inSampler, uint32_t inDstArrayElement = 0) override;

		VkDescriptorSet GetVkSet() const;
		void SetVkSet(VkDescriptorSet inSet);

		VulkanDescriptorSet& API_VULKAN() override;

	private :
		VulkanDevice& vulkanDevice;
		VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
	};
}
