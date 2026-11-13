#include "Interface/device.hpp"
#include "Interface/device_queue.hpp"
#include "Interface/physical_device.hpp"
#include <stdexcept>

namespace rhi
{
	Device::Device(Instance* inInstance, PhysicalDevice* inPhysicalDevice, Surface* inSurface) : physicalDevice(inPhysicalDevice)
	{
	}

	VulkanDevice& Device::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanDevice");
	}

	PhysicalDevice* Device::GetPhysicalDevice()
	{
		return physicalDevice;
	}

	DeviceQueue* Device::GetGraphicQueue()
	{
		return graphicQueue.get();
	}

	DeviceQueue* Device::GetPresentQueue()
	{
		return presentQueue.get();
	}

	DeviceQueue* Device::GetComputeQueue()
	{
		return computeQueue.get();
	}

	DeviceQueue* Device::GetPrimaryTransferQueue()
	{
		return primaryTransferQueue.get();
	}

	DeviceQueue* Device::GetSecondaryTransferQueue()
	{
		return secondaryTransferQueue.get();
	}
}
