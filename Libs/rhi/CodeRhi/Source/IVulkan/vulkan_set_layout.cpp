#include "IVulkan/vulkan_set_layout.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"
#include "Volk/volk.h"
#include <stdexcept>

namespace rhi
{
	VulkanSetLayout::VulkanSetLayout(Device* inDevice, const SetLayoutDesc& inDesc) :
		SetLayout(inDevice, inDesc),
		vulkanDevice(inDevice->API_VULKAN())
	{
		AllocateLayout();
	}

	VulkanSetLayout::~VulkanSetLayout()
	{
		FreeLayout();
	}

	void VulkanSetLayout::AllocateLayout()
	{
		uint32_t count = static_cast<uint32_t>(desc.SetInfos.size());
		if (count == 0)
		{
			return;
		}

		std::vector<VkDescriptorSetLayoutBinding> bindings(count);
		std::vector<VkDescriptorBindingFlags> bindingFlags(count, 0);

		for (uint32_t i = 0; i < count; i++)
		{
			const auto& info = desc.SetInfos[i];
			bindings[i].binding = info.Binding;
			bindings[i].descriptorCount = info.Count;
			bindings[i].descriptorType = ToVulkanDescriptorType(info.Type);
			bindings[i].stageFlags = ToVulkanShaderStage(info.ShaderStages);
			bindings[i].pImmutableSamplers = nullptr;

			if (info.Variable)
			{
				bindingFlags[i] = VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT;
			}
		}

		VkDescriptorSetLayoutBindingFlagsCreateInfo flagsCreateInfo{};
		flagsCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
		flagsCreateInfo.bindingCount = count;
		flagsCreateInfo.pBindingFlags = bindingFlags.data();

		VkDescriptorSetLayoutCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.bindingCount = count;
		createInfo.pBindings = bindings.data();
		createInfo.pNext = &flagsCreateInfo;

		if (vkCreateDescriptorSetLayout(vulkanDevice.GetVkDevice(), &createInfo, nullptr, &setLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan descriptor set layout");
		}
	}

	void VulkanSetLayout::FreeLayout()
	{
		if (setLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(vulkanDevice.GetVkDevice(), setLayout, nullptr);
			setLayout = VK_NULL_HANDLE;
		}
	}

	VkDescriptorSetLayout VulkanSetLayout::GetLayout() const
	{
		return setLayout;
	}

	VulkanSetLayout& VulkanSetLayout::API_VULKAN()
	{
		return *this;
	}
}
