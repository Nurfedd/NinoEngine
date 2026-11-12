#include "IVulkan/enums/vulkan_rasterizer_state.hpp"

namespace rhi 
{
	VkPolygonMode ToVulkanPolygonMode(PolygonMode polygonMode) 
	{
		return gPolygonModeToVk[static_cast<uint32_t>(polygonMode)];
	}

	PolygonMode FromVulkanPolygonMode(VkPolygonMode vkPolygonMode) 
	{
		switch (vkPolygonMode) 
		{
		case VK_POLYGON_MODE_FILL:  return PolygonMode::Fill;
		case VK_POLYGON_MODE_LINE:  return PolygonMode::Line;
		case VK_POLYGON_MODE_POINT: return PolygonMode::Point;
		default: return PolygonMode::Fill;
		}
	}

	VkFrontFace ToVulkanFrontFace(FrontFace frontFace) 
	{
		return gFrontFaceToVk[static_cast<uint32_t>(frontFace)];
	}

	FrontFace FromVulkanFrontFace(VkFrontFace vkFrontFace) 
	{
		switch (vkFrontFace) 
		{
		case VK_FRONT_FACE_CLOCKWISE: return FrontFace::Clockwise;
		case VK_FRONT_FACE_COUNTER_CLOCKWISE: return FrontFace::CounterClockwise;
		default: return FrontFace::Clockwise;
		}
	}

	VkCullModeFlags ToVulkanCullModeFlags(CullModeFlags cullModeFlags) 
	{
		VkCullModeFlags flags = 0;

		if (HasFlag(cullModeFlags, CullModeFlags::BackFace))
			flags |= VK_CULL_MODE_BACK_BIT;

		if (HasFlag(cullModeFlags, CullModeFlags::FrontFace))
			flags |= VK_CULL_MODE_FRONT_BIT;

		return flags;
	}

	CullModeFlags FromVulkanCullModeFlags(VkCullModeFlags vkCullModeFlags) 
	{
		CullModeFlags flags = CullModeFlags::None;

		if (vkCullModeFlags & VK_CULL_MODE_BACK_BIT)
			flags |= CullModeFlags::BackFace;

		if (vkCullModeFlags & VK_CULL_MODE_FRONT_BIT)
			flags |= CullModeFlags::FrontFace;

		return flags;
	}
}