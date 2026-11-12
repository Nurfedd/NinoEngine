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

		// Alias compatibilité
		PolygonMode& polygonMode = Mode;
		FrontFace& frontFace = Face;
		CullModeFlags& cullModeFlags = CullFlags;
		Format& depthFormat = DepthTextureFormat;
		ColorAttachmentDescriptor*& colorAttachments = ColorAttachments;
		uint32_t& attachmentCount = AttachmentCount;
		bool& depthTestEnable = DepthTestEnable;
		bool& depthWrite = DepthWrite;
		DepthOperator& depthOperator = DepthOp;
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

		// Alias compatibilité
		PushConstantRange*& pushConstants = PushConstants;
		SetLayout**& setLayouts = SetLayouts;
		VertexLayout*& vertexLayouts = VertexLayouts;
		uint32_t& pushConstantCount = PushConstantCount;
		uint32_t& setLayoutCount = SetLayoutCount;
		uint32_t& vertexLayoutCount = VertexLayoutCount;
		RasterizerInfo& rasterizerInfo = RasterInfo;
	};

	struct GraphicVertexPipelineCreateInfo : public GraphicPipelineCreateInfo
	{
	public :
		Shader* VertexShader = nullptr;
		Shader* FragmentShader = nullptr;
		Shader* GeometryShader = nullptr;

		// Alias compatibilité
		Shader*& vertexShader = VertexShader;
		Shader*& fragmentShader = FragmentShader;
		Shader*& geometryShader = GeometryShader;
	};

	struct GraphicMeshPipelineCreateInfo : public GraphicPipelineCreateInfo
	{
	public :
		Shader* FragmentShader = nullptr;
		Shader* TaskShader = nullptr;
		Shader* MeshShader = nullptr;

		// Alias compatibilité
		Shader*& fragmentShader = FragmentShader;
		Shader*& taskShader = TaskShader;
		Shader*& meshShader = MeshShader;
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
