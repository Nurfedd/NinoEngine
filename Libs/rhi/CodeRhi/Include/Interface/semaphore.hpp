#pragma once

#include "Interface/rhi_resource.hpp"
#include <cstdint>

namespace rhi
{
	class VulkanSemaphore;

	enum class SemaphoreType
	{
		Binary,
		Timeline
	};

	inline constexpr SemaphoreType BINARY_SEMAPHORE = SemaphoreType::Binary;
	inline constexpr SemaphoreType TIMELINE_SEMAPHORE = SemaphoreType::Timeline;

	struct SemaphoreDesc
	{
	public :
		SemaphoreDesc() = default;
		SemaphoreDesc(SemaphoreType inType, uint64_t inInitialValue = 0) :
			Type(inType),
			InitialValue(inInitialValue)
		{
		}

	public :
		SemaphoreType Type = SemaphoreType::Binary;
		uint64_t InitialValue = 0;
	};

	class Semaphore : public RHIResource
	{
	public :
		Semaphore(Device* inDevice, const SemaphoreDesc& inDesc);
		~Semaphore() override = default;

		virtual uint64_t GetTimelineValue() = 0;
		virtual bool IsBinary() const;
		virtual VulkanSemaphore& API_VULKAN();

	protected :
		SemaphoreDesc desc;
	};
}
