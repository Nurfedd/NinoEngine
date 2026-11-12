#pragma once

namespace rhi
{
	enum class PresentMode
	{
		Immediate,
		Mailbox,
		Fifo
	};

	enum class Format
	{
		Undefined = 0,
		RGBA16_SFLOAT,
		R8_UNORM,
		RG8_UNORM,
		RGB8_UNORM,
		RGBA8_UNORM,
		RGBA16_UNORM,
		R8_SRGB,
		RG8_SRGB,
		RGB8_SRGB,
		RGBA8_SRGB,
		BGRA8_UNORM,
		BGRA8_SRGB,

		D32_FLOAT,
		D24_UNORM_S8_UINT
	};

	enum class DepthOperator
	{
		Never = 0,
		Less = 1,
		Equal = 2,
		LessOrEqual = 3,
		Greater = 4,
		NotEqual = 5,
		GreaterOrEqual = 6,
		Always = 7
	};

	enum class ColorSpace
	{
		SrgbNonLinear
	};

	struct SurfaceFormat
	{
		SurfaceFormat() = default;
		SurfaceFormat(Format inFormat, ColorSpace inColorSpace) { format = inFormat; colorSpace = inColorSpace; }
		Format format;
		ColorSpace colorSpace;
	};
}
