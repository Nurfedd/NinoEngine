#include "Interface/swapchain.hpp"
#include "Interface/surface.hpp"
#include "Interface/texture.hpp"
namespace rhi {
	Swapchain::Swapchain(Device* inDevice, SwapchainCreateInfo swapchainCreateInfo) : RHIResource(inDevice)
	{
		presentMode = swapchainCreateInfo.presentMode;
		surfaceFormat = swapchainCreateInfo.surfaceFormat;
		surface = swapchainCreateInfo.surface;
	}
	

	void Swapchain::CreateImages() {
		swapchainImageTextures.resize(imageCount);
	}
}