#include "IVulkan/vulkan_swapchain.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_surface.hpp"
#include "IVulkan/vulkan_texture.hpp"
#include "IVulkan/vulkan_device_queue.hpp"
#include "IVulkan/enums/vulkan_format.hpp"
#include "IVulkan/vulkan_fence.hpp"
#include "IVulkan/vulkan_semaphore.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "Volk/volk.h"
#include "enums/format.hpp"
#include <algorithm>
#include <stdexcept>

namespace rhi
{

	VulkanSwapchain::VulkanSwapchain(Device* inDevice, SwapchainCreateInfo swapchainCreateInfo) : Swapchain(inDevice,swapchainCreateInfo)
	{
		VulkanDevice& vulkanDevice = inDevice->API_VULKAN();
		if (surface == nullptr)
		{
			throw std::runtime_error("Surface is null for Swapchain creation");
		}
		VulkanSurface& vulkanSurface = surface->API_VULKAN();
		

		VkSurfaceCapabilitiesKHR surfaceCapabilites;
		vulkanSurface.GetCapabilities(vulkanDevice.GetVkPhysicalDevice(), &surfaceCapabilites);

		DeviceQueue* graphicQueue = vulkanDevice.GetGraphicQueue();
		DeviceQueue* presentQueue = vulkanDevice.GetPresentQueue();

		if (graphicQueue == nullptr || presentQueue == nullptr)
		{
			throw std::runtime_error("Cannot create a Swapchain without present queue and graphic queue");
		}

		if (!surface->SupportPresentMode(presentMode) || !surface->SupportSurfaceFormat(surfaceFormat))
		{
			presentMode = PresentMode::Fifo;
			surfaceFormat = SurfaceFormat(Format::RGBA8_SRGB, ColorSpace::SrgbNonLinear);
		}
		imageCount = std::max(uint32_t(1), swapchainCreateInfo.frameInFlight);
		if (surfaceCapabilites.maxImageCount != 0)
		{
			imageCount = std::min(imageCount, surfaceCapabilites.maxImageCount);
		}
		Recreate();
	}


	void VulkanSwapchain::Recreate()
	{
		
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		DestroyImages();
		VulkanSurface& vulkanSurface = surface->API_VULKAN();
		
		VkSurfaceCapabilitiesKHR capabilities;
		vulkanSurface.GetCapabilities(vulkanDevice.GetVkPhysicalDevice(), &capabilities);
		swapchainExtent = capabilities.currentExtent;
		VkSwapchainKHR oldSwapchain = swapchain;

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = vulkanSurface.GetSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = ToVulkanFormat(surfaceFormat.format);
		createInfo.imageColorSpace = ToVulkanColorSpace(surfaceFormat.colorSpace);
		createInfo.imageExtent = capabilities.currentExtent;
		createInfo.preTransform = capabilities.currentTransform;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = ToVulkanPresentMode(presentMode);
		createInfo.clipped = VK_TRUE;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.oldSwapchain = oldSwapchain;

		if (vkCreateSwapchainKHR(vulkanDevice, &createInfo, nullptr, &swapchain) != VK_SUCCESS)
		{
			throw std::runtime_error("Swapchain creation failed");
		}

		if (oldSwapchain)
		{
			vkDestroySwapchainKHR(vulkanDevice, oldSwapchain, nullptr);
		}
		CreateImages();
	}

	SwapchainAcquireResult VulkanSwapchain::AcquireImage(Fence* fence, Semaphore* semaphore)
	{
		SwapchainAcquireResult acquireResult{};
		VulkanDevice& vulkanDevice = device->API_VULKAN();

		if (semaphore && !semaphore->IsBinary())
		{
			throw std::runtime_error("Semaphore must be a binary semaphore for acquire image");
		}
		VkResult result = vkAcquireNextImageKHR(
			vulkanDevice,
			swapchain,
			UINT64_MAX,
			(semaphore == nullptr) ? VK_NULL_HANDLE : semaphore->API_VULKAN().GetSemaphore(),
			(fence == nullptr) ? VK_NULL_HANDLE : fence->API_VULKAN().GetFence(),
			&imageIndex
		);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			acquireResult.success = false;
		}
		else
		{
			acquireResult.success = true;
			acquireResult.imageIndex = imageIndex;
		}
		return acquireResult;
	}

	void VulkanSwapchain::PreparePresentImage(CommandBuffer* commandBuffer)
	{
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer->API_VULKAN();
		VulkanTexture& vulkanPresentTexture = swapchainImageTextures[imageIndex]->API_VULKAN();
		vulkanPresentTexture.GetImage()->TransitionImage(
			vulkanCommandBuffer,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT,
			0,
			VK_IMAGE_ASPECT_COLOR_BIT
		);
	}

	void VulkanSwapchain::CreateImages()
	{
		Swapchain::CreateImages();
		VulkanDevice& vulkanDevice = device->API_VULKAN();

		uint32_t count = 0;
		vkGetSwapchainImagesKHR(vulkanDevice, swapchain, &count, nullptr);
		std::vector<VkImage> vkImages(count);
		vkGetSwapchainImagesKHR(vulkanDevice, swapchain, &count, vkImages.data());

		for (uint32_t i = 0; i < count; i++)
		{
			VulkanImage image;
			image.CreateFromVkImage(vkImages[i], ToVulkanFormat(surfaceFormat.format));
			image.SetStage(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT, 0);

			VulkanImageViewCreateInfo imageViewCreateInfo;
			imageViewCreateInfo.image = &image;
			imageViewCreateInfo.format = ToVulkanFormat(surfaceFormat.format);
			imageViewCreateInfo.layerCount = 1;
			imageViewCreateInfo.mipCount = 1;
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

			VulkanImageView imageView;
			imageView.Create(vulkanDevice, imageViewCreateInfo);

			TextureDesc desc;
			desc.Width = GetWidth();
			desc.Height = GetHeight();
			desc.TextureFormat = surfaceFormat.format;
			desc.Usage = ImageUsage::ColorAttachment;
			desc.MipCount = 1;
			desc.Type = ViewType::View2D;
			
			swapchainImageTextures[i] = std::make_unique<VulkanTexture>(device, desc, image, imageView);
		}
	}

	void VulkanSwapchain::DestroyImages()
	{
		swapchainImageTextures.clear();
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		VulkanDevice& vulkanDevice = device->API_VULKAN();
		DestroyImages();
		if (swapchain != VK_NULL_HANDLE)
		{
			vkDestroySwapchainKHR(vulkanDevice, swapchain, nullptr);
			swapchain = VK_NULL_HANDLE;
		}
	}
}
