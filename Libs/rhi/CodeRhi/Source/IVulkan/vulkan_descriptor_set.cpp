#include "IVulkan/vulkan_descriptor_set.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_buffer.hpp"
#include "IVulkan/vulkan_texture.hpp"
#include "IVulkan/vulkan_sampler.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"
#include "Volk/volk.h"
#include <vector>

namespace rhi
{
	VulkanDescriptorSet::VulkanDescriptorSet(Device* inDevice) :
		DescriptorSet(inDevice),
		vulkanDevice(inDevice->API_VULKAN())
	{
	}

	void VulkanDescriptorSet::UpdateBuffer(uint32_t inBinding, BufferUpdateInfo inBufferUpdateInfo, DescriptorType inDescriptorType)
	{
		uint32_t count = inBufferUpdateInfo.Count;
		std::vector<VkDescriptorBufferInfo> bufferInfos(count);

		for (uint32_t i = 0; i < count; i++)
		{
			VulkanBuffer& vulkanBuffer = inBufferUpdateInfo.Buffers[i]->API_VULKAN();
			bufferInfos[i].buffer = vulkanBuffer.GetBuffer();
			bufferInfos[i].offset = 0;
			bufferInfos[i].range = vulkanBuffer.GetVkBufferSize();
		}

		VkWriteDescriptorSet writeDescriptorSetInfo{};
		writeDescriptorSetInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSetInfo.dstBinding = inBinding;
		writeDescriptorSetInfo.descriptorType = ToVulkanDescriptorType(inDescriptorType);
		writeDescriptorSetInfo.descriptorCount = count;
		writeDescriptorSetInfo.dstSet = descriptorSet;
		writeDescriptorSetInfo.pBufferInfo = bufferInfos.data();
		writeDescriptorSetInfo.dstArrayElement = inBufferUpdateInfo.DstArrayElement;

		vkUpdateDescriptorSets(vulkanDevice.GetVkDevice(), 1, &writeDescriptorSetInfo, 0, nullptr);
	}

	void VulkanDescriptorSet::UpdateTexture(uint32_t inBinding, Texture* inTexture, Sampler* inSampler, uint32_t inDstArrayElement)
	{
		VulkanTexture& vulkanTexture = inTexture->API_VULKAN();
		VulkanSampler& vulkanSampler = inSampler->API_VULKAN();

		VkDescriptorImageInfo imageInfo{};
		imageInfo.sampler = vulkanSampler.GetSampler();
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = vulkanTexture.GetImageView()->GetVkImageView();

		VkWriteDescriptorSet writeDescriptorSetInfo{};
		writeDescriptorSetInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSetInfo.descriptorCount = 1;
		writeDescriptorSetInfo.dstBinding = inBinding;
		writeDescriptorSetInfo.dstSet = descriptorSet;
		writeDescriptorSetInfo.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeDescriptorSetInfo.pImageInfo = &imageInfo;
		writeDescriptorSetInfo.dstArrayElement = inDstArrayElement;

		vkUpdateDescriptorSets(vulkanDevice.GetVkDevice(), 1, &writeDescriptorSetInfo, 0, nullptr);
	}

	VkDescriptorSet VulkanDescriptorSet::GetVkSet() const
	{
		return descriptorSet;
	}

	void VulkanDescriptorSet::SetVkSet(VkDescriptorSet inSet)
	{
		descriptorSet = inSet;
	}

	VulkanDescriptorSet& VulkanDescriptorSet::API_VULKAN()
	{
		return *this;
	}
}
