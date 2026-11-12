#pragma once

#include "nino_core.hpp"

namespace rhi
{
	enum class BufferUsage
	{
		UniformBuffer = 1 << 1,
		StorageBuffer = 1 << 2,
		VertexBuffer = 1 << 3,
		IndexBuffer = 1 << 4,
		IndirectCommandBuffer = 1 << 5,
		TransferSrc = 1 << 6,
		TransferDst = 1 << 7
	};
	ENUM_CLASS_FLAGS(BufferUsage);

	enum class BufferState
	{
		Undefined,
		ComputeWrite,
		ComputeRead,
		VertexBuffer,
		IndexBuffer,
		IndirectDraw,
		TransferSrc,
		TransferDst
	};
}
