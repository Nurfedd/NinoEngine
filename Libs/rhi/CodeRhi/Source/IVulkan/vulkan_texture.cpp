#include "IVulkan/vulkan_texture.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/vulkan_buffer.hpp"
#include "IVulkan/enums/vulkan_format.hpp"
#include "IVulkan/enums/vulkan_image_type.hpp"
#include "IVulkan/enums/vulkan_synchronization_stage.hpp"
#include <algorithm>
#include <stdexcept>
#include "volk.h"
namespace rhi
{
	VulkanTextureCreateInfo::VulkanTextureCreateInfo(const TextureDesc& inDesc)
	{
		Width = inDesc.Width;
		Height = inDesc.Height;
		TextureFormat = ToVulkanFormat(inDesc.TextureFormat);
		ImageUsage = ToVulkanImageUsage(inDesc.Usage);
		MipCount = inDesc.MipCount;
		ViewType = ToVulkanViewType(inDesc.Type);
	}

	VulkanTexture::VulkanTexture(Device* inDevice, const TextureDesc& inDesc) :
		Texture(inDevice, inDesc),
		vulkanDevice(inDevice->API_VULKAN()),
		ownsVulkanImage(true)
	{
		AllocateTexture();
	}

	VulkanTexture::VulkanTexture(Device* inDevice, const TextureDesc& inDesc, VulkanImage inImage, VulkanImageView inImageView) :
		Texture(inDevice, inDesc),
		vulkanDevice(inDevice->API_VULKAN()),
		vulkanImage(inImage),
		vulkanImageView(inImageView),
		ownsVulkanImage(false)
	{
	}

	VulkanTexture::~VulkanTexture()
	{
		FreeTexture();
	}

	void VulkanTexture::AllocateTexture()
	{
		vkImageUsage = ToVulkanImageUsage(desc.Usage) | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		VulkanImageCreateInfo imageCreateInfo{};
		imageCreateInfo.width = desc.Width;
		imageCreateInfo.height = desc.Height;
		imageCreateInfo.format = ToVulkanFormat(desc.TextureFormat);
		imageCreateInfo.usageFlags = vkImageUsage;
		imageCreateInfo.mipLevel = desc.MipCount;
		imageCreateInfo.arrayLayers = GetLayerCountByViewType(desc.Type);

		vulkanImage.Create(vulkanDevice, imageCreateInfo);
		vulkanImageView.Create(vulkanDevice, vulkanImage, ToVulkanViewType(desc.Type));
	}

	void VulkanTexture::FreeTexture()
	{
		if (ownsVulkanImage)
		{
			vulkanImageView.Destroy(vulkanDevice);
			vulkanImage.Destroy(vulkanDevice);
		}
	}

	
	void VulkanTexture::Upload(CommandBuffer* inCommandBuffer, const void* inPixels, uint32_t inChannels)
	{
		if (inCommandBuffer == nullptr || inPixels == nullptr)
		{
			return;
		}

		VulkanCommandBuffer& cmd = inCommandBuffer->API_VULKAN();
		size_t imageSize = static_cast<size_t>(desc.Width) * desc.Height * inChannels;

		BufferDesc stagingDesc;
		stagingDesc.Size = imageSize;
		stagingDesc.Usage = BufferUsage::TransferSrc;
		stagingDesc.IsCpuAccessible = true;

		VulkanBuffer stagingBuffer(device, stagingDesc);
		stagingBuffer.Upload(inCommandBuffer,inPixels, imageSize, 0);

		vulkanImage.TransitionImage(
			cmd,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_PIPELINE_STAGE_2_TRANSFER_BIT,
			VK_ACCESS_2_TRANSFER_WRITE_BIT
		);

		VkBufferImageCopy copyRegion{};
		copyRegion.bufferOffset = 0;
		copyRegion.bufferRowLength = 0;
		copyRegion.bufferImageHeight = 0;
		copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copyRegion.imageSubresource.mipLevel = 0;
		copyRegion.imageSubresource.baseArrayLayer = 0;
		copyRegion.imageSubresource.layerCount = vulkanImage.GetArrayLayerCount();
		copyRegion.imageOffset = {0, 0, 0};
		copyRegion.imageExtent = {desc.Width, desc.Height, 1};

		vkCmdCopyBufferToImage(
			cmd.commandBuffer,
			stagingBuffer.GetBuffer(),
			vulkanImage.GetVkImage(),
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&copyRegion
		);

		if (desc.MipCount > 1)
		{
			GenerateMipmaps(cmd);
		}
	}

	void VulkanTexture::GenerateMipmaps(VulkanCommandBuffer& inCmd)
	{
		int32_t mipWidth = static_cast<int32_t>(desc.Width);
		int32_t mipHeight = static_cast<int32_t>(desc.Height);

		for (uint32_t i = 1; i < desc.MipCount; i++)
		{
			vulkanImage.TransitionImage(
				inCmd,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				VK_PIPELINE_STAGE_2_TRANSFER_BIT,
				VK_ACCESS_2_TRANSFER_READ_BIT,
				i - 1,
				1
			);

			vulkanImage.TransitionImage(
				inCmd,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_PIPELINE_STAGE_2_TRANSFER_BIT,
				VK_ACCESS_2_TRANSFER_WRITE_BIT,
				i,
				1
			);

			VkImageBlit blit{};
			blit.srcOffsets[0] = {0, 0, 0};
			blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
			blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel = i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount = vulkanImage.GetArrayLayerCount();

			blit.dstOffsets[0] = {0, 0, 0};
			blit.dstOffsets[1] = {std::max(1, mipWidth / 2), std::max(1, mipHeight / 2), 1};
			blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel = i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount = vulkanImage.GetArrayLayerCount();

			vkCmdBlitImage(
				inCmd.commandBuffer,
				vulkanImage.GetVkImage(),
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				vulkanImage.GetVkImage(),
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&blit,
				VK_FILTER_LINEAR
			);

			mipWidth = std::max(1, mipWidth / 2);
			mipHeight = std::max(1, mipHeight / 2);
		}
	}

	void VulkanTexture::Resize(uint32_t inWidth, uint32_t inHeight)
	{
		if (desc.Width == inWidth && desc.Height == inHeight)
		{
			return;
		}

		vulkanDevice.WaitIdle();
		FreeTexture();
		desc.Width = inWidth;
		desc.Height = inHeight;
		AllocateTexture();
	}

	bool VulkanTexture::CanBeUsedInShader()
	{
		return vulkanImage.GetCurrentLayout() == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

	VulkanImage* VulkanTexture::GetImage()
	{
		return &vulkanImage;
	}

	VulkanImageView* VulkanTexture::GetImageView()
	{
		return &vulkanImageView;
	}

	VulkanTexture& VulkanTexture::API_VULKAN()
	{
		return *this;
	}
}
