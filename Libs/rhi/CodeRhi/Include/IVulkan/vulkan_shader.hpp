#pragma once

#include "Interface/shader.hpp"
#include "vulkan/vulkan.h"
#include <string>

namespace rhi
{
	class VulkanDevice;

	class VulkanShader : public Shader
	{
	public :
		VulkanShader(Device* inDevice, const ShaderDesc& inDesc);
		~VulkanShader() override;

		VkShaderModule GetShaderModule() const;
		VkShaderStageFlagBits GetShaderStage() const;
		VulkanShader& API_VULKAN() override;

	private :
		void AllocateShader();
		void FreeShader();

	private :
		VulkanDevice& vulkanDevice;
		VkShaderModule shaderModule = VK_NULL_HANDLE;
		VkShaderStageFlagBits shaderStage = VK_SHADER_STAGE_ALL;
	};
}
