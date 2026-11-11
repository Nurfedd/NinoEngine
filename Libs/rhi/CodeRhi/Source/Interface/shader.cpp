#include "Interface/shader.hpp"
#include <stdexcept>

namespace rhi
{
	Shader::Shader(Device* inDevice, const ShaderDesc& inDesc) :
		RHIResource(inDevice),
		desc(inDesc)
	{
	}

	const std::filesystem::path& Shader::GetFilePath() const
	{
		return desc.FilePath;
	}

	ShaderStagesFlags Shader::GetStage() const
	{
		return desc.Stage;
	}

	VulkanShader& Shader::API_VULKAN()
	{
		throw std::runtime_error("Bad API Call: object is not a VulkanShader");
	}
}
