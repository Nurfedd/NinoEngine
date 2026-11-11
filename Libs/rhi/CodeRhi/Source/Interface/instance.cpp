#include "Interface/instance.hpp"

namespace rhi {
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