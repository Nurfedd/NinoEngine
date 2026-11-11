#pragma once

#include "Interface/rhi_resource.hpp"
#include <cstdint>

namespace rhi
{
	class VulkanFence;

	struct FenceDesc
	{
	public :
		FenceDesc() = default;
		FenceDesc(bool inSignaled) :
			Signaled(inSignaled)
		{
		}

	public :
		bool Signaled = true;
	};

	class Fence : public RHIResource
	{
	public :
		Fence(Device* inDevice, const FenceDesc& inDesc);
		~Fence() override = default;

		virtual bool IsReady() = 0;
		virtual void Reset() = 0;
		virtual void Wait(uint64_t inTimeout = UINT64_MAX) = 0;

		virtual VulkanFence& API_VULKAN();

	protected :
		FenceDesc desc;
	};
}
