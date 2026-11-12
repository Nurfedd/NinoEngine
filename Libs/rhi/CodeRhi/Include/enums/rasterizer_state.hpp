#pragma once

#include "nino_core.hpp"

namespace rhi
{
	enum class PolygonMode
	{
		Fill,
		Line,
		Point
	};

	enum class FrontFace
	{
		Clockwise,
		CounterClockwise
	};

	enum class CullModeFlags
	{
		None = 0,
		BackFace = 1 << 1,
		FrontFace = 1 << 2,
		All = 0x7FFFFFFF
	};
	ENUM_CLASS_FLAGS(CullModeFlags);
}
