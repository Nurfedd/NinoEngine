#pragma once
#include "rhi_resource.hpp"
#include "enums/format.hpp"
#include <vector>
#include <memory>

namespace rhi {
	class VulkanSwapchain;
	class Surface;
	class Texture;
	class Fence;
	class Semaphore;
	class CommandBuffer;
	enum class PresentMode;

	struct SwapchainCreateInfo {
		Surface* surface;
		SurfaceFormat surfaceFormat;
		PresentMode presentMode;
		uint32_t frameInFlight;
	};
	struct SwapchainAcquireResult {
		uint32_t imageIndex;
		bool success;
	};
	class Swapchain : public RHIResource {
	public :
		Swapchain(Device* inDevice, SwapchainCreateInfo swapchainCreateInfo);
		~Swapchain() override = default;
		
		virtual SwapchainAcquireResult AcquireImage(Fence* fence,Semaphore* semaphore) = 0;
		virtual void PreparePresentImage(CommandBuffer* commandBuffer) = 0;
		void AdvanceToNextImage() { currentFrame = (currentFrame + 1) % imageCount; }
		virtual void Recreate() = 0;
		
		uint32_t GetImageCount() const { return imageCount; }
		uint32_t GetCurrentFrame() const { return currentFrame; }
		uint32_t GetImageIndex() const { return imageIndex; }
		Texture* GetTexture(uint32_t index) { return swapchainImageTextures[index].get(); }
		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual VulkanSwapchain& API_VULKAN() { throw std::exception("Bad API Call: object is not a VulkanSwapchain"); }
	protected :
		uint32_t imageCount = 0;
		uint32_t currentFrame = 0;
		uint32_t imageIndex = 0;

		PresentMode presentMode;
		SurfaceFormat surfaceFormat;
		Surface* surface;

		std::vector<std::unique_ptr<Texture>> swapchainImageTextures;

		virtual void DestroyImages() = 0;
		virtual void CreateImages();
	};
}