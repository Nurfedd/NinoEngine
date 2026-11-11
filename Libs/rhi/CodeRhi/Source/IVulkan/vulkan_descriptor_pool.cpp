#include "IVulkan/vulkan_descriptor_pool.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_set_layout.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"
#include "Volk/volk.h"
#include <unordered_map>
#include <stdexcept>
#include <vector>

namespace rhi
{
	VulkanDescriptorPool::VulkanDescriptorPool(Device* inDevice, const DescriptorPoolDesc& inDesc) :
		DescriptorPool(inDevice, inDesc),
		vulkanDevice(inDevice->API_VULKAN())
	{
		AllocatePool();
	}

	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		FreePool();
	}

	void VulkanDescriptorPool::AllocatePool()
	{
		if (desc.MaxSets == 0 || desc.Layouts.empty())
		{
			return;
		}

		std::unordered_map<VkDescriptorType, uint32_t> typeCounts;
		for (auto* layout : desc.Layouts)
		{
			if (layout == nullptr)
			{
				continue;
			}
			for (const auto& pair : layout->GetLayoutInfo())
			{
				
				typeCounts[ToVulkanDescriptorType(pair.first)] += pair.second * desc.MaxSets;
			}
		}

		std::vector<VkDescriptorPoolSize> poolSizes;
		for (const auto& pair : typeCounts)
		{
			VkDescriptorPoolSize sizeInfo{};
			sizeInfo.type = pair.first;
			sizeInfo.descriptorCount = pair.second;
			poolSizes.push_back(sizeInfo);
		}

		VkDescriptorPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
		createInfo.maxSets = desc.MaxSets;
		createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		createInfo.pPoolSizes = poolSizes.data();

		if (vkCreateDescriptorPool(vulkanDevice.GetVkDevice(), &createInfo, nullptr, &pool) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan descriptor pool");
		}
	}

	void VulkanDescriptorPool::FreePool()
	{
		if (pool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(vulkanDevice.GetVkDevice(), pool, nullptr);
			pool = VK_NULL_HANDLE;
		}
	}

	void VulkanDescriptorPool::AllocateSets(DescriptorSet** outSets, SetLayout** inLayouts, uint32_t inSetCount, uint32_t* inVariableCounts)
	{
		if (inSetCount == 0 || outSets == nullptr || inLayouts == nullptr)
		{
			return;
		}

		std::vector<VkDescriptorSetLayout> vkLayouts(inSetCount);
		std::vector<VkDescriptorSet> vkSets(inSetCount);

		for (uint32_t i = 0; i < inSetCount; i++)
		{
			vkLayouts[i] = inLayouts[i]->API_VULKAN().GetLayout();
		}

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = inSetCount;
		allocInfo.pSetLayouts = vkLayouts.data();

		VkDescriptorSetVariableDescriptorCountAllocateInfo countInfo{};
		if (inVariableCounts != nullptr)
		{
			countInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO;
			countInfo.descriptorSetCount = inSetCount;
			countInfo.pDescriptorCounts = inVariableCounts;
			allocInfo.pNext = &countInfo;
		}

		if (vkAllocateDescriptorSets(vulkanDevice.GetVkDevice(), &allocInfo, vkSets.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate descriptor sets");
		}

		for (uint32_t i = 0; i < inSetCount; i++)
		{
			outSets[i]->API_VULKAN().SetVkSet(vkSets[i]);
		}
	}

	void VulkanDescriptorPool::Reset()
	{
		if (pool != VK_NULL_HANDLE)
		{
			vkResetDescriptorPool(vulkanDevice.GetVkDevice(), pool, 0);
		}
	}

	void VulkanDescriptorPool::CreatePoolForImGui()
	{
		FreePool();

		VkDescriptorPoolSize pool_sizes[] = {
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		createInfo.maxSets = 11 * 1000;
		createInfo.poolSizeCount = 11;
		createInfo.pPoolSizes = pool_sizes;

		if (vkCreateDescriptorPool(vulkanDevice.GetVkDevice(), &createInfo, nullptr, &pool) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create ImGui Vulkan descriptor pool");
		}
	}

	VkDescriptorPool VulkanDescriptorPool::GetPool() const
	{
		return pool;
	}

	VulkanDescriptorPool& VulkanDescriptorPool::API_VULKAN()
	{
		return *this;
	}
}
