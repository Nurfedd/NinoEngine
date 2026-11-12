#include "IVulkan/vulkan_sampler.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/enums/vulkan_image_type.hpp"
#include "Volk/volk.h"
#include <stdexcept>

namespace rhi
{
	VulkanSampler::VulkanSampler(Device* inDevice, const SamplerDesc& inDesc) :
		Sampler(inDevice, inDesc),
		vulkanDevice(inDevice->API_VULKAN())
	{
		AllocateSampler();
	}

	VulkanSampler::~VulkanSampler()
	{
		FreeSampler();
	}

	void VulkanSampler::AllocateSampler()
	{
		VkSamplerCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		createInfo.magFilter = ToVulkanFilter(desc.Filter);
		createInfo.minFilter = ToVulkanFilter(desc.Filter);
		createInfo.addressModeU = ToVulkanAdressMode(desc.AddressModeU);
		createInfo.addressModeV = ToVulkanAdressMode(desc.AddressModeV);
		createInfo.addressModeW = ToVulkanAdressMode(desc.AddressModeW);
		createInfo.anisotropyEnable = desc.UseAnisotropy ? VK_TRUE : VK_FALSE;
		createInfo.maxAnisotropy = desc.UseAnisotropy ? 16.0f : 1.0f;
		createInfo.compareEnable = desc.CompareEnable ? VK_TRUE : VK_FALSE;
		createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		createInfo.mipmapMode = desc.UseMipmap ? VK_SAMPLER_MIPMAP_MODE_LINEAR : VK_SAMPLER_MIPMAP_MODE_NEAREST;
		createInfo.minLod = 0.0f;
		createInfo.maxLod = static_cast<float>(desc.MaxMipLevel);

		if (vkCreateSampler(vulkanDevice.GetVkDevice(), &createInfo, nullptr, &sampler) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan sampler");
		}
	}

	void VulkanSampler::FreeSampler()
	{
		if (sampler != VK_NULL_HANDLE)
		{
			vkDestroySampler(vulkanDevice.GetVkDevice(), sampler, nullptr);
			sampler = VK_NULL_HANDLE;
		}
	}

	VkSampler VulkanSampler::GetSampler() const
	{
		return sampler;
	}

	VulkanSampler& VulkanSampler::API_VULKAN()
	{
		return *this;
	}
}
