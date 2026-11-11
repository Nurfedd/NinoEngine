#pragma once

#include "Interface/rhi_resource.hpp"
#include "enums/format.hpp"
#include "enums/image_type.hpp"
#include "enums/synchronization_stage.hpp"
#include <memory>
#include <cstdint>

namespace rhi
{
	class CommandBuffer;
	class UploadContext;
	class VulkanTexture;

	struct TextureDesc
	{
	public :
		TextureDesc() = default;
		TextureDesc(uint32_t inWidth, uint32_t inHeight, Format inFormat, ImageUsage inUsage, uint32_t inMipCount = 1, ViewType inViewType = ViewType::View2D) :
			Width(inWidth),
			Height(inHeight),
			TextureFormat(inFormat),
			Usage(inUsage),
			MipCount(inMipCount),
			Type(inViewType)
		{
		}

	public :
		uint32_t Width = 0;
		uint32_t Height = 0;
		Format TextureFormat = Format::Undefined;
		ImageUsage Usage = ImageUsage::None;
		uint32_t MipCount = 1;
		ViewType Type = ViewType::View2D;
	};

	using TextureCreateInfo = TextureDesc;

	class Texture : public RHIResource
	{
	public :
		Texture(Device* inDevice, const TextureDesc& inDesc);
		~Texture() override = default;

		virtual void Upload(const void* inPixels, uint32_t inChannels) = 0;
		virtual void Upload(CommandBuffer* inCommandBuffer, const void* inPixels, uint32_t inChannels) = 0;
		virtual void Resize(uint32_t inWidth, uint32_t inHeight) = 0;
		virtual bool MakeImageReadableInShader(CommandBuffer* inCommandBuffer, PipelineStageFlags inDstStage, AccessFlags inAccessFlags) = 0;
		virtual bool CanBeUsedInShader() = 0;

		virtual VulkanTexture& API_VULKAN();

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;
		Format GetFormat() const;
		ImageUsage GetUsage() const;
		uint32_t GetMipCount() const;
		ViewType GetViewType() const;

	protected :
		TextureDesc desc;
	};
}
