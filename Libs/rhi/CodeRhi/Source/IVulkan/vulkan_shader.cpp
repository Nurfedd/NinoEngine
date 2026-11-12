#include "IVulkan/vulkan_shader.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/enums/vulkan_input_format.hpp"
#include "Volk/volk.h"
#include <fstream>
#include <vector>
#include <stdexcept>

namespace rhi
{
	VulkanShader::VulkanShader(Device* inDevice, const ShaderDesc& inDesc) :
		Shader(inDevice, inDesc),
		vulkanDevice(inDevice->API_VULKAN())
	{
		AllocateShader();
	}

	VulkanShader::~VulkanShader()
	{
		FreeShader();
	}

	void VulkanShader::AllocateShader()
	{
		if (desc.FilePath.empty())
		{
			return;
		}

		std::ifstream file(desc.FilePath, std::ios::ate | std::ios::binary);
		if (!file.is_open())
		{
			throw std::runtime_error("Cannot open shader file: " + desc.FilePath.string());
		}

		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> byteBuffer(fileSize);
		file.seekg(0);
		file.read(byteBuffer.data(), fileSize);
		file.close();

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pCode = reinterpret_cast<const uint32_t*>(byteBuffer.data());
		createInfo.codeSize = fileSize;

		if (vkCreateShaderModule(vulkanDevice.GetVkDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Vulkan shader module: " + desc.FilePath.string());
		}

		shaderStage = static_cast<VkShaderStageFlagBits>(ToVulkanShaderStage(desc.Stage));
	}

	void VulkanShader::FreeShader()
	{
		if (shaderModule != VK_NULL_HANDLE)
		{
			vkDestroyShaderModule(vulkanDevice.GetVkDevice(), shaderModule, nullptr);
			shaderModule = VK_NULL_HANDLE;
		}
	}

	VkShaderModule VulkanShader::GetShaderModule() const
	{
		return shaderModule;
	}

	VkShaderStageFlagBits VulkanShader::GetShaderStage() const
	{
		return shaderStage;
	}

	VulkanShader& VulkanShader::API_VULKAN()
	{
		return *this;
	}
}
