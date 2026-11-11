#pragma once
#include "Interface/render_interface.hpp"


namespace rhi {
	class VulkanInterface : public RenderInterface {
	public:
		virtual std::shared_ptr<Instance> CreateInstance(const char* applicationName) override;
		virtual std::unique_ptr<Surface> CreateSurface(std::shared_ptr<Instance> instance, GLFWwindow* window) override;
	};
}