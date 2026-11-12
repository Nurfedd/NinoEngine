#pragma once

#include "Interface/rhi_resource.hpp"
#include "enums/input_fomat.hpp"
#include <vector>
#include <cstdint>

namespace rhi
{
	class Buffer;
	class Texture;
	class Sampler;
	class VulkanDescriptorSet;

	struct BufferUpdateInfo
	{
	public :
		BufferUpdateInfo() = default;
		BufferUpdateInfo(Buffer** inBuffers, uint32_t inCount = 1, uint32_t inDstArrayElement = 0) :
			Buffers(inBuffers),
			Count(inCount),
			DstArrayElement(inDstArrayElement)
		{
		}

	public :
		Buffer** Buffers = nullptr;
		uint32_t Count = 1;
		uint32_t DstArrayElement = 0;

		// Alias compatibilité
		Buffer**& buffers = Buffers;
		uint32_t& count = Count;
		uint32_t& dstArrayElement = DstArrayElement;
	};

	class DescriptorSet : public RHIResource
	{
	public :
		DescriptorSet(Device* inDevice);
		~DescriptorSet() override = default;

		virtual void UpdateBuffer(uint32_t inBinding, BufferUpdateInfo inBufferUpdateInfo, DescriptorType inDescriptorType) = 0;
		virtual void UpdateTexture(uint32_t inBinding, Texture* inTexture, Sampler* inSampler, uint32_t inDstArrayElement = 0) = 0;

		virtual VulkanDescriptorSet& API_VULKAN();
	};
}
