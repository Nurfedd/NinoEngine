#pragma once

#include "Interface/sampler.hpp"
#include "vulkan/vulkan.h"

namespace rhi
{
	class VulkanDevice;

	struct SamplerCreateInfoVulkan
	{
	public :
		VkFilter filter = VK_FILTER_LINEAR;
		VkSamplerAddressMode adressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		VkSamplerAddressMode adressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		VkSamplerAddressMode adressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		float maxLod = 0.0f;
		float minLod = 0.0f;
		bool compareEnable = false;
		bool useAnisotropy = false;
		bool useMipmap = false;
	};

	class VulkanSampler : public Sampler
	{
	public :
		VulkanSampler(Device* inDevice, const SamplerDesc& inDesc);
		~VulkanSampler() override;

		VkSampler GetSampler() const;
		VulkanSampler& API_VULKAN() override;

	private :
		void AllocateSampler();
		void FreeSampler();

	private :
		VulkanDevice& vulkanDevice;
		VkSampler sampler = VK_NULL_HANDLE;
	};
}
