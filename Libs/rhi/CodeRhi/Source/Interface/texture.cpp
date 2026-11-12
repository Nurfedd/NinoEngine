#include "Interface/texture.hpp"
#include <stdexcept>

namespace rhi
{
	Texture::Texture(Device* inDevice, const TextureDesc& inDesc) :
		RHIResource(inDevice),
		desc(inDesc)
	{
	}

	uint32_t Texture::GetWidth() const
	{
		return desc.Width;
	}

	uint32_t Texture::GetHeight() const
	{
		return desc.Height;
	}

	Format Texture::GetFormat() const
	{
		return desc.TextureFormat;
	}

	ImageUsage Texture::GetUsage() const
	{
		return desc.Usage;
	}

	uint32_t Texture::GetMipCount() const
	{
		return desc.MipCount;
	}

	ViewType Texture::GetViewType() const
	{
		return desc.Type;
	}

	VulkanTexture& Texture::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanTexture");
	}
}
