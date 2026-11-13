#pragma once

#include "nino_core.hpp"
#include <optional>
#include <cstdint>

namespace rhi
{
	class Surface;
	class VulkanPhysicalDevice;

	enum class QueueSupportedFlags
	{
		None = 0,
		GraphicsSupport = 1 << 1,
		ComputeSupport = 1 << 2,
		TransferSupport = 1 << 3
	};
	ENUM_CLASS_FLAGS(QueueSupportedFlags);

	enum class QueueType
	{
		GraphicQueue,
		ComputeQueue,
		PresentQueue,
		TransferQueue
	};

	class PhysicalDevice
	{
	public :
		virtual ~PhysicalDevice() = default;

		virtual bool SupportSurface(Surface* inSurface) = 0;
		virtual VulkanPhysicalDevice& API_VULKAN();

	public :
		bool IsDiscreteGpu = false;
		QueueSupportedFlags Flags = QueueSupportedFlags::None;

	protected :
		virtual void LoadInfo() = 0;
	};
}
