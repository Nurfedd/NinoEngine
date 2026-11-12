#pragma once
#include <memory>
#include <vector>
#include <cstdint>
#include "nino_core.hpp"

namespace rhi
{
	class Instance;
	class PhysicalDevice;
	class Surface;
	class DeviceQueue;
	class VulkanDevice;

	class Buffer;
	class Texture;
	class Fence;
	class Semaphore;
	class Sampler;
	class Shader;
	class SetLayout;
	class DescriptorPool;
	class DescriptorSet;
	class GraphicPipeline;
	class ComputePipeline;
	class CommandPool;
	class DynamicRenderer;

	class Device
	{
	public :
		Device(Instance* inInstance, PhysicalDevice* inPhysicalDevice, Surface* inSurface);
		virtual ~Device() = default;

		NINO_NONCOPYABLE(Device);
		
		virtual void WaitIdle() = 0;
		virtual VulkanDevice& API_VULKAN();

		virtual void WaitForFences(Fence** inFences, uint32_t inCount, bool inWaitForAll, uint64_t inTimeout = UINT64_MAX) = 0;
		virtual void ResetFences(Fence** inFences, uint32_t inCount) = 0;

		virtual std::unique_ptr<Buffer> CreateBuffer(const struct BufferDesc& inDesc) = 0;
		virtual std::unique_ptr<Texture> CreateTexture(const struct TextureDesc& inDesc) = 0;
		virtual std::unique_ptr<Fence> CreateFence(const struct FenceDesc& inDesc) = 0;
		virtual std::unique_ptr<Semaphore> CreateSemaphore(const struct SemaphoreDesc& inDesc) = 0;
		virtual std::unique_ptr<Sampler> CreateSampler(const struct SamplerDesc& inDesc) = 0;
		virtual std::unique_ptr<Shader> CreateShader(const struct ShaderDesc& inDesc) = 0;
		virtual std::unique_ptr<SetLayout> CreateSetLayout(const struct SetLayoutDesc& inDesc) = 0;
		virtual std::unique_ptr<DescriptorPool> CreateDescriptorPool(const struct DescriptorPoolDesc& inDesc) = 0;
		virtual std::unique_ptr<DescriptorSet> CreateDescriptorSet() = 0;
		virtual std::unique_ptr<GraphicPipeline> CreateGraphicPipeline() = 0;
		virtual std::unique_ptr<ComputePipeline> CreateComputePipeline() = 0;
		virtual std::unique_ptr<CommandPool> CreateCommandPool(const struct CommandPoolDesc& inDesc) = 0;
		virtual std::unique_ptr<DynamicRenderer> CreateDynamicRenderer() = 0;

		PhysicalDevice* GetPhysicalDevice();
		DeviceQueue* GetGraphicQueue();
		DeviceQueue* GetPresentQueue();
		DeviceQueue* GetComputeQueue();
		DeviceQueue* GetPrimaryTransferQueue();
		DeviceQueue* GetSecondaryTransferQueue();

	protected :
		std::shared_ptr<DeviceQueue> graphicQueue;
		std::shared_ptr<DeviceQueue> presentQueue;
		std::unique_ptr<DeviceQueue> computeQueue;
		std::unique_ptr<DeviceQueue> primaryTransferQueue;
		std::unique_ptr<DeviceQueue> secondaryTransferQueue;
		PhysicalDevice* physicalDevice = nullptr;
	};
}
