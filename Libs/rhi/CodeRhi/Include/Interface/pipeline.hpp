#pragma once

#include "Interface/rhi_resource.hpp"
#include "enums/input_fomat.hpp"
#include <cstddef>
#include <cstdint>

namespace rhi
{
	class CommandBuffer;
	class DescriptorSet;

	class Pipeline : public RHIResource
	{
	public :
		Pipeline(Device* inDevice);
		~Pipeline() override = default;

		virtual void PushConstant(CommandBuffer* inCommandBuffer, uint32_t inOffset, size_t inSize, const void* inData, ShaderStagesFlags inShaderStages) = 0;
		virtual void Bind(CommandBuffer* inCommandBuffer) = 0;
		virtual void BindDescriptorSet(CommandBuffer* inCommandBuffer, DescriptorSet** inSets, uint32_t inSetCount, uint32_t inFirstSet = 0) = 0;

		void PushConstant(CommandBuffer* inCommandBuffer, uint32_t inOffset, size_t inSize, void* inData, ShaderStagesFlags inShaderStages)
		{
			PushConstant(inCommandBuffer, inOffset, inSize, const_cast<const void*>(inData), inShaderStages);
		}
	};
}
