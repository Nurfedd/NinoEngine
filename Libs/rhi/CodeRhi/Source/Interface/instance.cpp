#include "Interface/instance.hpp"
#include "Interface/physical_device.hpp"
#include "Interface/device.hpp"
namespace rhi {
	Instance::Instance(const char* applicationName) 
	{

	}
	std::vector<PhysicalDevice*> Instance::GetPhysicalDevices() {
		if (!hasLoadedDevice)
			LoadPhysicalDevices();
		
		std::vector<PhysicalDevice*> physicalDevices;
		for (std::unique_ptr<PhysicalDevice>& physicalDevice : physicalDeviceViews)
		{
			physicalDevices.push_back(physicalDevice.get());
		}
		return physicalDevices;
	}
}