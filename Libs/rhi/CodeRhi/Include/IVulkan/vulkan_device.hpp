#pragma once

#include "Interface/device.hpp"
#include "IVulkan/vk_mem_alloc.h"


namespace rhi
{
	
	class VulkanDevice : public Device
	{
	public :
		VulkanDevice(Instance* inInstance, PhysicalDevice* inPhysicalDevice, Surface* inSurface);
		~VulkanDevice() override;

		
		void WaitIdle() override;

		void WaitForFences(Fence** fences, uint32_t count, bool waitForAll, uint64_t timeout = UINT64_MAX) override;
		void ResetFences(Fence** fences, uint32_t count) override;

		std::unique_ptr<Buffer> CreateBuffer(const BufferDesc& inDesc) override;
		std::unique_ptr<Texture> CreateTexture(const TextureDesc& inDesc) override;
		std::unique_ptr<Fence> CreateFence(const FenceDesc& inDesc) override;
		std::unique_ptr<Semaphore> CreateSemaphore(const SemaphoreDesc& inDesc) override;
		std::unique_ptr<Sampler> CreateSampler(const SamplerDesc& inDesc) override;
		std::unique_ptr<Shader> CreateShader(const ShaderDesc& inDesc) override;
		std::unique_ptr<SetLayout> CreateSetLayout(const SetLayoutDesc& inDesc) override;
		std::unique_ptr<DescriptorPool> CreateDescriptorPool(const DescriptorPoolDesc& inDesc) override;
		std::unique_ptr<DescriptorSet> CreateDescriptorSet() override;
		std::unique_ptr<GraphicPipeline> CreateGraphicPipeline() override;
		std::unique_ptr<ComputePipeline> CreateComputePipeline() override;
		std::unique_ptr<CommandPool> CreateCommandPool(const struct CommandPoolDesc& inDesc) override;
		std::unique_ptr<DynamicRenderer> CreateDynamicRenderer() override;

		void DestroyBuffer(VkBuffer inBuffer, VmaAllocation inAllocation);
		void DestroyImage(VkImage inImage, VmaAllocation inAllocation);
		void DestroyImageView(VkImageView inImageView);

		VulkanDevice& API_VULKAN() override;

		VkPhysicalDevice GetVkPhysicalDevice() const;
		VkDevice GetVkDevice() const;
		VmaAllocator GetAllocator() const;

		operator VkDevice()
		{
			return logicalDevice;
		}

	private :
		void InitAllocator(VkInstance inInstance);
		void InitPhysicalDeviceProperties();

	private :
		VkPhysicalDeviceProperties2 physicalDeviceProperties{};
		VmaAllocator allocator = nullptr;
		VkDevice logicalDevice = VK_NULL_HANDLE;
	};
}
