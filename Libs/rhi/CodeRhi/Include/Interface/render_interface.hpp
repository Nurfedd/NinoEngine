#pragma once
#include <memory>

class GLFWwindow;

namespace rhi {
	class Instance;
	class Surface;

	class RenderInterface {
	public:
		// Create
		virtual std::shared_ptr<Instance> CreateInstance(const char* applicationName) = 0;
		virtual std::unique_ptr<Surface> CreateSurface(std::shared_ptr<Instance> instance,GLFWwindow* window) = 0;
		
	};
}