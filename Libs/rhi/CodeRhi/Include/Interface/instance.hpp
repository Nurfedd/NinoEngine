#pragma once
#include <exception>
#include <memory>
#include <vector>
#include "nino_core.hpp"

namespace rhi {
	class VulkanInstance;
	class PhysicalDevice;
	class Surface;
	class Device;

	class Instance {
	public :
		Instance(const char* applicationName) {}
		virtual ~Instance() = default;

		NINO_NONCOPYABLE(Instance);

		virtual std::unique_ptr<Device> CreateDevice(PhysicalDevice* inPhysicalDevice, Surface* inSurface) = 0;
		std::vector<PhysicalDevice*> GetPhysicalDevices();
		virtual VulkanInstance& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanInstance"); }

	protected :
		bool hasLoadedDevice = false;
		std::vector<std::unique_ptr<PhysicalDevice>> physicalDeviceViews;
		virtual void LoadPhysicalDevices() = 0;
		
	};
}