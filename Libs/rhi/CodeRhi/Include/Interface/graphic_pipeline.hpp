#pragma once

#include "Interface/pipeline.hpp"
#include "Interface/vertex_layout.hpp"
#include "Interface/push_constant_range.hpp"
#include "enums/rasterizer_state.hpp"
#include "enums/format.hpp"
#include <vector>

namespace rhi
{
	class Device;
	class SetLayout;
	class Shader;
	class VulkanGraphicPipeline;

	struct ColorAttachmentDescriptor
	{
	public :
		Format TextureFormat = Format::Undefined;
		bool EnableBlending = false;

		// Alias compatibilité
		Format& format = TextureFormat;
		bool& enableBlending = EnableBlending;
	};

	struct RasterizerInfo
	{
	public :
		PolygonMode Mode = PolygonMode::Fill;
		FrontFace Face = FrontFace::Clockwise;
		CullModeFlags CullFlags = CullModeFlags::BackFace;
		Format DepthTextureFormat = Format::Undefined;
		ColorAttachmentDescriptor* ColorAttachments = nullptr;
		uint32_t AttachmentCount = 0;
		bool DepthTestEnable = false;
		bool DepthWrite = false;
		DepthOperator DepthOp = DepthOperator::Less;

	};

	struct GraphicPipelineCreateInfo
	{
	public :
		PushConstantRange* PushConstants = nullptr;
		SetLayout** SetLayouts = nullptr;
		VertexLayout* VertexLayouts = nullptr;
		uint32_t PushConstantCount = 0;
		uint32_t SetLayoutCount = 0;
		uint32_t VertexLayoutCount = 0;
		RasterizerInfo RasterInfo;

		
	};

	struct GraphicVertexPipelineCreateInfo : public GraphicPipelineCreateInfo
	{
	public :
		Shader* VertexShader = nullptr;
		Shader* FragmentShader = nullptr;
		Shader* GeometryShader = nullptr;

	};

	struct GraphicMeshPipelineCreateInfo : public GraphicPipelineCreateInfo
	{
	public :
		Shader* FragmentShader = nullptr;
		Shader* TaskShader = nullptr;
		Shader* MeshShader = nullptr;

	};

	class GraphicPipeline : public Pipeline
	{
	public :
		GraphicPipeline(Device* inDevice);
		~GraphicPipeline() override = default;

		virtual void Build(const GraphicVertexPipelineCreateInfo& inCreateInfo) = 0;
		virtual void Build(const GraphicMeshPipelineCreateInfo& inCreateInfo) = 0;

		virtual VulkanGraphicPipeline& API_VULKAN();
	};
}
