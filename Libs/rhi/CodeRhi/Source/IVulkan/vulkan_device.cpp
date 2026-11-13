#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_surface.hpp"
#include "IVulkan/vulkan_instance.hpp"
#include "IVulkan/vulkan_device_queue.hpp"
#include "IVulkan/vulkan_fence.hpp"
#include "IVulkan/vulkan_semaphore.hpp"
#include "IVulkan/vulkan_sampler.hpp"
#include "IVulkan/vulkan_shader.hpp"
#include "IVulkan/vulkan_set_layout.hpp"
#include "IVulkan/vulkan_descriptor_pool.hpp"
#include "IVulkan/vulkan_descriptor_set.hpp"
#include "IVulkan/vulkan_graphic_pipeline.hpp"
#include "IVulkan/vulkan_compute_pipeline.hpp"
#include "IVulkan/vulkan_command_pool.hpp"
#include "IVulkan/vulkan_dynamic_renderer.hpp"
#include "IVulkan/vulkan_buffer.hpp"
#include "IVulkan/vulkan_texture.hpp"
#include "Volk/volk.h"
#include <algorithm>
#include <stdexcept>

namespace rhi
{
	VulkanDevice::VulkanDevice(Instance* inInstance, PhysicalDevice* inPhysicalDevice, Surface* inSurface) : Device(inInstance,inPhysicalDevice,inSurface)
	{
		VulkanPhysicalDevice& vulkanPhysicalDevice = physicalDevice->API_VULKAN();
		VulkanInstance& vulkanInstance = inInstance->API_VULKAN();
		VkPhysicalDevice vkPhysicalDevice = GetVkPhysicalDevice();
		PhysicalDeviceQueueInfos physicalDeviceQueueInfos = vulkanPhysicalDevice.GetFamilyQueueIndices(inSurface);

		std::unordered_map<uint32_t, uint32_t> queueFamilyIndice = physicalDeviceQueueInfos.GetQueueFamilyIndiceAndCount();

		if (!(physicalDeviceQueueInfos.graphicQueue.IsValid() && physicalDeviceQueueInfos.computeQueue.IsValid()))
		{
			throw std::runtime_error("Physical device doesn't support required queue features.");
		}

		if (inSurface != nullptr && !(vulkanPhysicalDevice.FamilySupportPresent(physicalDeviceQueueInfos.graphicQueue.familyIndice, inSurface->API_VULKAN())))
		{
			throw std::runtime_error("Graphic family doesn't support present");
		}

		std::vector<VkDeviceQueueCreateInfo> queuesCreateInfos(queueFamilyIndice.size());
		std::vector<std::vector<float>> queuePriorities(queueFamilyIndice.size());
		int queueIdx = 0;

		for (auto& pair : queueFamilyIndice)
		{
			queuePriorities[queueIdx].resize(pair.second);
			std::fill(queuePriorities[queueIdx].begin(), queuePriorities[queueIdx].end(), 1.0f);
			VkDeviceQueueCreateInfo queueInfo{};
			queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueInfo.queueCount = pair.second;
			queueInfo.queueFamilyIndex = pair.first;
			queueInfo.pQueuePriorities = queuePriorities[queueIdx].data();
			queuesCreateInfos[queueIdx] = queueInfo;
			queueIdx++;
		}

		VkPhysicalDeviceDynamicRenderingFeatures dynamicRendering{};
		dynamicRendering.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES;
		dynamicRendering.dynamicRendering = VK_TRUE;

		VkPhysicalDeviceVulkan12Features vulkan12Features{};
		vulkan12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
		vulkan12Features.bufferDeviceAddress = VK_TRUE;
		vulkan12Features.timelineSemaphore = VK_TRUE;

		VkPhysicalDeviceSynchronization2Features synchronizationFeatures{};
		synchronizationFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES;
		synchronizationFeatures.synchronization2 = VK_TRUE;

		VkPhysicalDeviceFeatures2 features2{};
		features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		features2.features.samplerAnisotropy = VK_TRUE;

		VkPhysicalDeviceMeshShaderFeaturesEXT meshShaderFeatures{};
		meshShaderFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_EXT;
		meshShaderFeatures.meshShader = VK_TRUE;
		meshShaderFeatures.taskShader = VK_TRUE;
		meshShaderFeatures.multiviewMeshShader = VK_FALSE;
		meshShaderFeatures.primitiveFragmentShadingRateMeshShader = VK_FALSE;
		meshShaderFeatures.meshShaderQueries = VK_FALSE;

		VkPhysicalDeviceDescriptorHeapFeaturesEXT descriptorHeapFeature{};
		descriptorHeapFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_HEAP_FEATURES_EXT;
		descriptorHeapFeature.descriptorHeap = VK_TRUE;
		descriptorHeapFeature.descriptorHeapCaptureReplay = VK_FALSE;

		features2.pNext = &dynamicRendering;
		dynamicRendering.pNext = &vulkan12Features;
		vulkan12Features.pNext = &synchronizationFeatures;
		synchronizationFeatures.pNext = &meshShaderFeatures;
		meshShaderFeatures.pNext = &descriptorHeapFeature;
		descriptorHeapFeature.pNext = nullptr;


		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queuesCreateInfos.size());
		createInfo.pQueueCreateInfos = queuesCreateInfos.data();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();
		createInfo.pNext = &features2;

		if (vkCreateDevice(vkPhysicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create logical Vulkan Device!");
		}
		volkLoadDevice(logicalDevice);

		graphicQueue = std::make_shared<VulkanDeviceQueue>(logicalDevice, physicalDeviceQueueInfos.graphicQueue.familyIndice);
		presentQueue = graphicQueue;
		computeQueue = std::make_unique<VulkanDeviceQueue>(logicalDevice, physicalDeviceQueueInfos.computeQueue.familyIndice);

		if (physicalDeviceQueueInfos.transferQueueCpuToGpu.IsValid())
		{
			primaryTransferQueue = std::make_unique<VulkanDeviceQueue>(logicalDevice, physicalDeviceQueueInfos.transferQueueCpuToGpu.familyIndice);
		}
		if (physicalDeviceQueueInfos.transferQueueGpuToCpu.IsValid())
		{
			secondaryTransferQueue = std::make_unique<VulkanDeviceQueue>(logicalDevice, physicalDeviceQueueInfos.transferQueueGpuToCpu.familyIndice);
		}

		InitPhysicalDeviceProperties();
		InitAllocator(vulkanInstance.GetVkInstance());
	}

	VulkanDevice::~VulkanDevice()
	{
		if (allocator != nullptr)
		{
			vmaDestroyAllocator(allocator);
			allocator = nullptr;
		}
		if (logicalDevice != VK_NULL_HANDLE)
		{
			vkDestroyDevice(logicalDevice, nullptr);
			logicalDevice = VK_NULL_HANDLE;
		}
	}

	void VulkanDevice::WaitForFences(Fence** fences, uint32_t count, bool waitForAll, uint64_t timeout)
	{
		std::vector<VkFence> vkFences(count);
		for (uint32_t i = 0; i < count; i++)
		{
			vkFences[i] = fences[i]->API_VULKAN().GetFence();
		}
		vkWaitForFences(logicalDevice, count, vkFences.data(), waitForAll, timeout);
	}

	void VulkanDevice::ResetFences(Fence** fences, uint32_t count)
	{
		std::vector<VkFence> vkFences(count);
		for (uint32_t i = 0; i < count; i++)
		{
			vkFences[i] = fences[i]->API_VULKAN().GetFence();
		}
		vkResetFences(logicalDevice, count, vkFences.data());
	}

	std::unique_ptr<Buffer> VulkanDevice::CreateBuffer(const BufferDesc& inDesc)
	{
		return std::make_unique<VulkanBuffer>(this, inDesc);
	}

	std::unique_ptr<Texture> VulkanDevice::CreateTexture(const TextureDesc& inDesc)
	{
		return std::make_unique<VulkanTexture>(this, inDesc);
	}

	std::unique_ptr<Fence> VulkanDevice::CreateFence(const FenceDesc& inDesc)
	{
		return std::make_unique<VulkanFence>(this, inDesc);
	}

	std::unique_ptr<Semaphore> VulkanDevice::CreateSemaphore(const SemaphoreDesc& inDesc)
	{
		return std::make_unique<VulkanSemaphore>(this, inDesc);
	}

	std::unique_ptr<Sampler> VulkanDevice::CreateSampler(const SamplerDesc& inDesc)
	{
		return std::make_unique<VulkanSampler>(this, inDesc);
	}

	std::unique_ptr<Shader> VulkanDevice::CreateShader(const ShaderDesc& inDesc)
	{
		return std::make_unique<VulkanShader>(this, inDesc);
	}

	std::unique_ptr<SetLayout> VulkanDevice::CreateSetLayout(const SetLayoutDesc& inDesc)
	{
		return std::make_unique<VulkanSetLayout>(this, inDesc);
	}

	std::unique_ptr<DescriptorPool> VulkanDevice::CreateDescriptorPool(const DescriptorPoolDesc& inDesc)
	{
		return std::make_unique<VulkanDescriptorPool>(this, inDesc);
	}

	std::unique_ptr<DescriptorSet> VulkanDevice::CreateDescriptorSet()
	{
		return std::make_unique<VulkanDescriptorSet>(this);
	}

	std::unique_ptr<GraphicPipeline> VulkanDevice::CreateGraphicPipeline()
	{
		return std::make_unique<VulkanGraphicPipeline>(this);
	}

	std::unique_ptr<ComputePipeline> VulkanDevice::CreateComputePipeline()
	{
		return std::make_unique<VulkanComputePipeline>(this);
	}

	std::unique_ptr<CommandPool> VulkanDevice::CreateCommandPool(const CommandPoolDesc& inDesc)
	{
		return std::make_unique<VulkanCommandPool>(this,inDesc);
	}

	std::unique_ptr<DynamicRenderer> VulkanDevice::CreateDynamicRenderer()
	{
		return std::make_unique<VulkanDynamicRenderer>();
	}

	void VulkanDevice::DestroyBuffer(VkBuffer inBuffer, VmaAllocation inAllocation)
	{
		if (allocator != nullptr && inBuffer != VK_NULL_HANDLE && inAllocation != nullptr)
		{
			vmaDestroyBuffer(allocator, inBuffer, inAllocation);
		}
	}

	void VulkanDevice::DestroyImage(VkImage inImage, VmaAllocation inAllocation)
	{
		if (allocator != nullptr && inImage != VK_NULL_HANDLE && inAllocation != nullptr)
		{
			vmaDestroyImage(allocator, inImage, inAllocation);
		}
	}

	void VulkanDevice::DestroyImageView(VkImageView inImageView)
	{
		if (logicalDevice != VK_NULL_HANDLE && inImageView != VK_NULL_HANDLE)
		{
			vkDestroyImageView(logicalDevice, inImageView, nullptr);
		}
	}

	void VulkanDevice::InitAllocator(VkInstance inInstance)
	{
		VmaVulkanFunctions vkFunctions{};
		vkFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
		vkFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;
		vkFunctions.vkCreateImage = vkCreateImage;
		vkFunctions.vkCreateBuffer = vkCreateBuffer;

		VmaAllocatorCreateInfo allocatorCI{};
		allocatorCI.physicalDevice = GetVkPhysicalDevice();
		allocatorCI.device = logicalDevice;
		allocatorCI.pVulkanFunctions = &vkFunctions;
		allocatorCI.vkInstance = inInstance;
		allocatorCI.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
		if (vmaCreateAllocator(&allocatorCI, &allocator) != VK_SUCCESS)
		{
			throw std::runtime_error("Cannot create VmaAllocator");
		}
	}

	void VulkanDevice::InitPhysicalDeviceProperties()
	{
		vkGetPhysicalDeviceProperties2(GetVkPhysicalDevice(), &physicalDeviceProperties);
	}

	void VulkanDevice::WaitIdle()
	{
		if (logicalDevice != VK_NULL_HANDLE)
		{
			vkDeviceWaitIdle(logicalDevice);
		}
	}

	VulkanDevice& VulkanDevice::API_VULKAN()
	{
		return *this;
	}

	VkPhysicalDevice VulkanDevice::GetVkPhysicalDevice() const
	{
		return physicalDevice->API_VULKAN().GetVulkanPhysicalDevice();
	}

	VkDevice VulkanDevice::GetVkDevice() const
	{
		return logicalDevice;
	}

	VmaAllocator VulkanDevice::GetAllocator() const
	{
		return allocator;
	}
}
