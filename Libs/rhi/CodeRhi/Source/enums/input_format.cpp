#include "enums/input_fomat.hpp"

namespace rhi 
{
	size_t GetSizeFromInputFormat(InputFormat inputFormat)
	{
		switch (inputFormat)
		{
		case InputFormat::Float1:   return sizeof(float);
		case InputFormat::Float2:   return sizeof(float) * 2;
		case InputFormat::Float3:   return sizeof(float) * 3;
		case InputFormat::Float4:   return sizeof(float) * 4;

		case InputFormat::Int1:     return sizeof(int32_t);
		case InputFormat::Int2:     return sizeof(int32_t) * 2;
		case InputFormat::Int3:     return sizeof(int32_t) * 3;
		case InputFormat::Int4:     return sizeof(int32_t) * 4;

		case InputFormat::Uint32_1: return sizeof(uint32_t);
		case InputFormat::Uint32_2: return sizeof(uint32_t) * 2;
		case InputFormat::Uint32_3: return sizeof(uint32_t) * 3;
		case InputFormat::Uint32_4: return sizeof(uint32_t) * 4;

		case InputFormat::Uint64_1: return sizeof(uint64_t);
		case InputFormat::Uint64_2: return sizeof(uint64_t) * 2;
		case InputFormat::Uint64_4: return sizeof(uint64_t) * 4;

		default:
			return 0;
		}
	}
}