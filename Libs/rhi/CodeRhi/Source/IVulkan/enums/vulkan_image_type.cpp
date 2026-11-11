#include "IVulkan/enums/vulkan_image_type.hpp"

namespace rhi 
{
	VkImageViewType ToVulkanViewType(ViewType viewType) 
	{
		return gImageViewToVk[static_cast<uint32_t>(viewType)];
	}

	ViewType FromVulkanViewType(VkImageViewType vkViewType) 
	{
		switch (vkViewType)
		{
		case VK_IMAGE_VIEW_TYPE_2D: return ViewType::View2D;
		case VK_IMAGE_VIEW_TYPE_CUBE: return ViewType::ViewCube;
		default: return ViewType::View2D;
		}
	}

	VkImageUsageFlags ToVulkanImageUsage(ImageUsage usage)
	{
		VkImageUsageFlags flags = 0;

		if (HasFlag(usage, ImageUsage::TransferSrc))
			flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		if (HasFlag(usage, ImageUsage::TransferDst))
			flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		if (HasFlag(usage, ImageUsage::Sampled))
			flags |= VK_IMAGE_USAGE_SAMPLED_BIT;

		if (HasFlag(usage, ImageUsage::Storage))
			flags |= VK_IMAGE_USAGE_STORAGE_BIT;

		if (HasFlag(usage, ImageUsage::ColorAttachment))
			flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		if (HasFlag(usage, ImageUsage::DepthStencilAttachment))
			flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

		if (HasFlag(usage, ImageUsage::InputAttachment))
			flags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

		return flags;
	}

	VkSamplerAddressMode ToVulkanAdressMode(SamplerAdressMode samplerAdress) 
	{
		switch (samplerAdress)
		{
		case SamplerAddressMode::Repeat: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		case SamplerAddressMode::MirroredRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		case SamplerAddressMode::ClampToEdge: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		case SamplerAddressMode::ClampToBorder: return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		default: return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		}
	}

	VkFilter ToVulkanFilter(SamplerFilter samplerFilter) 
	{
		switch (samplerFilter)
		{
		case SamplerFilter::Linear: return VK_FILTER_LINEAR;
		case SamplerFilter::Nearest: return VK_FILTER_NEAREST;
		default: return VK_FILTER_LINEAR;
		}
	}
}