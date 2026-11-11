#include "Interface/set_layout.hpp"
#include <stdexcept>

namespace rhi
{
	SetLayout::SetLayout(Device* inDevice, const SetLayoutDesc& inDesc) :
		RHIResource(inDevice),
		desc(inDesc)
	{
		for (const auto& info : desc.SetInfos)
		{
			layoutInfos[info.Type] += info.Count;
		}
	}

	std::unordered_map<DescriptorType, uint32_t> SetLayout::GetLayoutInfo() const
	{
		return layoutInfos;
	}

	const SetLayoutDesc& SetLayout::GetDesc() const
	{
		return desc;
	}

	VulkanSetLayout& SetLayout::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanSetLayout");
	}
}
