#pragma once

#include "Interface/rhi_resource.hpp"
#include "enums/input_fomat.hpp"
#include <vector>
#include <cstdint>

namespace rhi
{
	class SetLayout;
	class DescriptorSet;
	class VulkanDescriptorPool;

	struct DescriptorPoolDesc
	{
	public :
		DescriptorPoolDesc() = default;
		DescriptorPoolDesc(std::vector<SetLayout*> inLayouts, uint32_t inMaxSets) :
			Layouts(std::move(inLayouts)),
			MaxSets(inMaxSets)
		{
		}

	public :
		std::vector<SetLayout*> Layouts;
		uint32_t MaxSets = 0;
	};

	class DescriptorPool : public RHIResource
	{
	public :
		DescriptorPool(Device* inDevice, const DescriptorPoolDesc& inDesc);
		~DescriptorPool() override = default;

		virtual std::vector<DescriptorSet*> AllocateSets(SetLayout** inLayouts, uint32_t inSetCount, uint32_t* inVariableCounts = nullptr) = 0;
		virtual void Reset() = 0;

		virtual VulkanDescriptorPool& API_VULKAN();

	protected :
		DescriptorPoolDesc desc;
	};
}
