#pragma once

#include "nino_core.hpp"

namespace rhi
{
	enum class PipelineStageFlags
	{
		None = 0,
		TopOfPipe = 1 << 0,
		DrawIndirect = 1 << 1,
		VertexInput = 1 << 2,
		VertexShader = 1 << 3,
		FragmentShader = 1 << 4,
		ComputeShader = 1 << 5,
		Transfer = 1 << 6,
		BottomOfPipe = 1 << 7,
		ColorAttachmentOutput = 1 << 8,
		LateFragmentTests = 1 << 9,
		AllCommands = 1 << 10
	};
	ENUM_CLASS_FLAGS(PipelineStageFlags);

	enum class AccessFlags
	{
		None = 0,
		TransferRead = 1 << 1,
		TransferWrite = 1 << 2,
		ShaderRead = 1 << 3,
		ShaderWrite = 1 << 4,
		VertexRead = 1 << 5,
		IndexRead = 1 << 6,
		UniformRead = 1 << 7,
		ColorAttachmentWrite = 1 << 8,
		ColorAttachmentRead = 1 << 9,
		DepthStencilWrite = 1 << 10
	};
	ENUM_CLASS_FLAGS(AccessFlags);

	enum class ImageLayout
	{
		Undefined = 0,
		General,
		ColorAttachment,
		DepthStencilAttachment,
		DepthStencilReadOnly,
		ShaderReadOnly,
		TransferSrc,
		TransferDst,
		Present
	};
}
