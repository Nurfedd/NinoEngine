#pragma once

#include <cstdint>
#include "nino_core.hpp"

namespace rhi
{
	enum class ViewType
	{
		View2D,
		ViewCube
	};

	enum class ImageUsage
	{
		None = 0,
		TransferSrc = 1 << 1,
		TransferDst = 1 << 2,
		Sampled = 1 << 3,
		Storage = 1 << 4,
		ColorAttachment = 1 << 5,
		DepthStencilAttachment = 1 << 6,
		InputAttachment = 1 << 7
	};
	ENUM_CLASS_FLAGS(ImageUsage);

	enum class SamplerAddressMode
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	using SamplerAdressMode = SamplerAddressMode;

	enum class SamplerFilter
	{
		Linear,
		Nearest
	};

	uint32_t GetLayerCountByViewType(ViewType inViewType);
}
