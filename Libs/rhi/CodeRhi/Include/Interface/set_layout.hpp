#pragma once

#include "Interface/rhi_resource.hpp"
#include "enums/input_fomat.hpp"
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace rhi
{
	class VulkanSetLayout;

	struct SetInfo
	{
	public :
		SetInfo() = default;
		SetInfo(uint32_t inBinding, uint32_t inCount, ShaderStagesFlags inStages, DescriptorType inType, bool inIsVariable = false) :
			Binding(inBinding),
			Count(inCount),
			ShaderStages(inStages),
			Type(inType),
			Variable(inIsVariable)
		{
		}

	public :
		uint32_t Binding = 0;
		uint32_t Count = 1;
		ShaderStagesFlags ShaderStages = ShaderStagesFlags::None;
		DescriptorType Type = DescriptorType::UniformBuffer;
		bool Variable = false;

		// Alias compatibilité
		uint32_t& binding = Binding;
		uint32_t& count = Count;
		ShaderStagesFlags& shaderStages = ShaderStages;
		DescriptorType& descriptorType = Type;
		bool& variable = Variable;
	};

	struct SetLayoutDesc
	{
	public :
		SetLayoutDesc() = default;
		SetLayoutDesc(std::vector<SetInfo> inSetInfos) :
			SetInfos(std::move(inSetInfos))
		{
		}

	public :
		std::vector<SetInfo> SetInfos;
	};

	class SetLayout : public RHIResource
	{
	public :
		SetLayout(Device* inDevice, const SetLayoutDesc& inDesc);
		~SetLayout() override = default;

		std::unordered_map<DescriptorType, uint32_t> GetLayoutInfo() const;
		const SetLayoutDesc& GetDesc() const;

		virtual VulkanSetLayout& API_VULKAN();

	protected :
		SetLayoutDesc desc;
		std::unordered_map<DescriptorType, uint32_t> layoutInfos;
	};
}
