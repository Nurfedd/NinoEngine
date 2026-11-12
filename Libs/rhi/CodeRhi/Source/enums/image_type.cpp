#include "enums/image_type.hpp"

namespace rhi 
{
	uint32_t GetLayerCountByViewType(ViewType viewType) 
	{
		switch (viewType)
		{
		case ViewType::View2D: return 1;
		case ViewType::ViewCube: return 6; 
		default: return 1;
		}
	}
}