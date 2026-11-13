#include "Interface/buffer.hpp"
#include <stdexcept>

namespace rhi
{
	Buffer::Buffer(Device* inDevice, const BufferDesc& inDesc) :
		RHIResource(inDevice),
		desc(inDesc)
	{
	}

	BufferDesc Buffer::GetBufferDesc() const
	{
		return desc;
	}

	size_t Buffer::GetSize() const
	{
		return desc.Size;
	}

	BufferUsage Buffer::GetUsage() const
	{
		return desc.Usage;
	}

	bool Buffer::IsCpuAccessible() const
	{
		return desc.IsCpuAccessible;
	}

	VulkanBuffer& Buffer::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanBuffer");
	}
}
