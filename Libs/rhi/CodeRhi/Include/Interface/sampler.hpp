#pragma once

#include "Interface/rhi_resource.hpp"
#include "enums/image_type.hpp"

namespace rhi
{
	class VulkanSampler;

	struct SamplerDesc
	{
	public :
		SamplerDesc() = default;

	public :
		SamplerFilter Filter = SamplerFilter::Linear;
		SamplerAddressMode AddressModeU = SamplerAddressMode::Repeat;
		SamplerAddressMode AddressModeV = SamplerAddressMode::Repeat;
		SamplerAddressMode AddressModeW = SamplerAddressMode::Repeat;
		int MaxMipLevel = 0;
		bool CompareEnable = false;
		bool UseAnisotropy = false;
		bool UseMipmap = false;

		// Alias compatibilité
		SamplerFilter& filter = Filter;
		SamplerAddressMode& adressModeU = AddressModeU;
		SamplerAddressMode& adressModeV = AddressModeV;
		SamplerAddressMode& adressModeW = AddressModeW;
		int& maxMipLevel = MaxMipLevel;
		bool& compareEnable = CompareEnable;
		bool& useAnisotropy = UseAnisotropy;
		bool& useMipmap = UseMipmap;
	};

	using SamplerCreateInfo = SamplerDesc;

	class Sampler : public RHIResource
	{
	public :
		Sampler(Device* inDevice, const SamplerDesc& inDesc);
		~Sampler() override = default;

		virtual VulkanSampler& API_VULKAN();

	protected :
		SamplerDesc desc;
	};
}
