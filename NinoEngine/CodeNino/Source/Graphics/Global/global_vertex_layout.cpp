#include "Graphics/Global/global_vertex_layout.hpp"
#include "Interface/graphic_api.hpp"
using namespace rhi;
namespace nino_engine {
	GlobalVertexLayout::GlobalVertexLayout(std::vector<VertexInput> vertexInputs, uint32_t binding, uint32_t stride) :
		vertexLayout(vertexInputs.data(),vertexInputs.size(),binding,stride)
	{

	}

	
}