#pragma once

#include "nino_core.hpp"
#include <cstddef>

namespace rhi
{
	enum class InputFormat
	{
		Float1,
		Float2,
		Float3,
		Float4,

		Int1,
		Int2,
		Int3,
		Int4,

		Uint32_1,
		Uint32_2,
		Uint32_3,
		Uint32_4,

		Uint64_1,
		Uint64_2,
		Uint64_4
	};

	enum class InputRate
	{
		Vertex,
		Instance
	};

	enum class DescriptorType
	{
		UniformBuffer,
		StorageBuffer,
		SampledTexture
	};

	enum class ShaderStagesFlags
	{
		None = 0,
		Vertex = 1 << 1,
		Fragment = 1 << 2,
		Geometry = 1 << 3,
		Graphics = Vertex | Fragment | Geometry,
		Compute = 1 << 4
	};
	ENUM_CLASS_FLAGS(ShaderStagesFlags);

	enum class IndexType
	{
		Uint8,
		Uint16,
		Uint32
	};

	size_t GetSizeFromInputFormat(InputFormat inInputFormat);
}
