#pragma once
#include "enums/format.hpp"
#include "nino_core.hpp"
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>
#include <memory>
namespace rhi {
	class VulkanSurface;
	class Device;
	class Instance;

	class Surface {
	public:
		Surface(std::shared_ptr<Instance> inInstance, GLFWwindow* window) : instance(inInstance) {};
		virtual ~Surface() = default;
		
		NINO_NONCOPYABLE(Surface)
		virtual void LoadFormats(Device* device) = 0;
		std::vector<PresentMode> GetPresentMode() {return presentModes;};
		std::vector<SurfaceFormat> GetSurfaceFormat() { return surfaceFormats; };

		bool SupportPresentMode(PresentMode presentMode);
		bool SupportSurfaceFormat(SurfaceFormat surfaceFormat);
		virtual VulkanSurface& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanSurface"); }
	protected :
		std::vector<PresentMode> presentModes;
		std::vector<SurfaceFormat> surfaceFormats;
		bool hasLoaded = false;
		std::shared_ptr<Instance> instance;
	};
}