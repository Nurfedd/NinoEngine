#pragma once

#include "Interface/rhi_resource.hpp"
#include "enums/input_fomat.hpp"
#include <filesystem>
#include <vector>
#include <cstdint>

namespace rhi
{
	class VulkanShader;

	struct ShaderDesc
	{
	public :
		ShaderDesc() = default;
		ShaderDesc(const std::filesystem::path& inFilePath, ShaderStagesFlags inStage = ShaderStagesFlags::None) :
			FilePath(inFilePath),
			Stage(inStage)
		{
		}

	public :
		std::filesystem::path FilePath;
		ShaderStagesFlags Stage = ShaderStagesFlags::None;
	};

	class Shader : public RHIResource
	{
	public :
		Shader(Device* inDevice, const ShaderDesc& inDesc);
		~Shader() override = default;

		const std::filesystem::path& GetFilePath() const;
		ShaderStagesFlags GetStage() const;

		virtual VulkanShader& API_VULKAN();

	protected :
		ShaderDesc desc;
	};
}
