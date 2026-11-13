#pragma once

#include "Interface/rhi_resource.hpp"
#include "enums/buffer_usage.hpp"
#include <memory>
#include <cstddef>
#include <cstdint>

namespace rhi
{
	class CommandBuffer;
	class VulkanBuffer;

	struct BufferDesc
	{
	public :
		BufferDesc() = default;
		BufferDesc(BufferUsage inUsage, size_t inSize, bool inIsCpuAccessible = false) :
			Usage(inUsage),
			Size(inSize),
			IsCpuAccessible(inIsCpuAccessible)
		{
		}

	public :
		BufferUsage Usage = static_cast<BufferUsage>(0);
		size_t Size = 0;
		bool IsCpuAccessible = false;
	};

	class Buffer : public RHIResource
	{
	public :
		Buffer(Device* inDevice, const BufferDesc& inDesc);
		~Buffer() override = default;

		virtual void Upload(CommandBuffer* commandBuffer,const void* inData, size_t inSize, size_t inOffset = 0) = 0;
		virtual void Read(void* outData, size_t inSize, size_t inOffset = 0) = 0;
		virtual void* GetMappedData() = 0;
		virtual void Transition(CommandBuffer* inCommandBuffer, BufferState inDstState) = 0;
		virtual uint64_t GetDeviceAddress() = 0;

		virtual VulkanBuffer& API_VULKAN();

		BufferDesc GetBufferDesc() const;
		size_t GetSize() const;
		BufferUsage GetUsage() const;
		bool IsCpuAccessible() const;

		size_t GetBufferSize() const
		{
			return GetSize();
		}

		bool IsCpuBuffer() const
		{
			return IsCpuAccessible();
		}

		bool IsGpuBuffer() const
		{
			return !IsCpuAccessible();
		}

		void* GetBufferData()
		{
			return GetMappedData();
		}

		void TransitionBuffer(CommandBuffer* inCommandBuffer, BufferState inDstState)
		{
			Transition(inCommandBuffer, inDstState);
		}

	protected :
		BufferDesc desc;
	};
}
