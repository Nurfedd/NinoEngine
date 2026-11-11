#pragma once

#include "vulkan/vulkan.h"
#include "Interface/texture.hpp"
#include "IVulkan/vulkan_image.hpp"
#include "IVulkan/vulkan_image_view.hpp"
#include <cstdint>

namespace rhi
{
	class VulkanDevice;
	class VulkanCommandBuffer;
	class VulkanUploadContext;

	struct VulkanTextureCreateInfo
	{
	public :
		VulkanTextureCreateInfo() = default;
		VulkanTextureCreateInfo(const TextureDesc& inDesc);

	public :
		uint32_t Width = 0;
		uint32_t Height = 0;
		VkFormat TextureFormat = VK_FORMAT_UNDEFINED;
		VkImageUsageFlags ImageUsage = 0;
		uint32_t MipCount = 1;
		VkImageViewType ViewType = VK_IMAGE_VIEW_TYPE_2D;
	};

	class VulkanTexture : public Texture
	{
	public :
		VulkanTexture(Device* inDevice, const TextureDesc& inDesc);
		VulkanTexture(Device* inDevice, const TextureDesc& inDesc, VulkanImage inImage, VulkanImageView inImageView);
		~VulkanTexture() override;

		void Upload(CommandBuffer* inCommandBuffer, const void* inPixels, uint32_t inChannels) override;
		
		void Resize(uint32_t inWidth, uint32_t inHeight) override;
		bool MakeImageReadableInShader(CommandBuffer* inCommandBuffer, PipelineStageFlags inDstStage, AccessFlags inAccessFlags) override;
		bool CanBeUsedInShader() override;

		VulkanImage* GetImage();
		VulkanImageView* GetImageView();
		VulkanTexture& API_VULKAN() override;

	private :
		void AllocateTexture();
		void FreeTexture();
		void GenerateMipmaps(VulkanCommandBuffer& inCmd);

	private :
		VulkanDevice& vulkanDevice;
		VulkanImage vulkanImage;
		VulkanImageView vulkanImageView;
		VkImageUsageFlags vkImageUsage = 0;
		bool ownsVulkanImage = true;
	};
}
