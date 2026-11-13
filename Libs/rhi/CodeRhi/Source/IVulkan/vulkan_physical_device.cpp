#include "IVulkan/vulkan_physical_device.hpp"
#include "IVulkan/vulkan_surface.hpp"
#include <string>
#include "Volk/volk.h"
#include <map>
namespace rhi {

    std::unordered_map<uint32_t, uint32_t> PhysicalDeviceQueueInfos::GetQueueFamilyIndiceAndCount() {
        std::unordered_map<uint32_t, uint32_t> familyIndiceCount;

        if (graphicQueue.IsValid()) {
            familyIndiceCount[graphicQueue.familyIndice]++;
        }
        if (computeQueue.IsValid()) {
            familyIndiceCount[computeQueue.familyIndice]++;
        }
        if (transferQueueCpuToGpu.IsValid()) {
            familyIndiceCount[transferQueueCpuToGpu.familyIndice]++;
        }
        if (transferQueueGpuToCpu.IsValid()) {
            familyIndiceCount[transferQueueGpuToCpu.familyIndice]++;
        }
        return familyIndiceCount;
    }
	void VulkanPhysicalDevice::Load(VkPhysicalDevice physicalDevice) {
		device = physicalDevice;
		LoadInfo();
	}
	void VulkanPhysicalDevice::LoadInfo() {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        IsDiscreteGpu = (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);


        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        queueFamilies.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (VkQueueFamilyProperties& familyProperty : queueFamilies) {
            if (familyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                Flags |= QueueSupportedFlags::GraphicsSupport;

            if (familyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT)
                Flags |= QueueSupportedFlags::ComputeSupport;
            if (familyProperty.queueFlags & VK_QUEUE_TRANSFER_BIT)
                Flags |= QueueSupportedFlags::TransferSupport;
        }
	}
	bool VulkanPhysicalDevice::SupportExtensions() {
        uint32_t availableExtensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(availableExtensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &availableExtensionCount, availableExtensions.data());
        bool supported = true;

        for (const std::string& required : deviceExtensions) {
            bool found = false;

            for (const VkExtensionProperties& available : availableExtensions) {
                if (required == available.extensionName) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                supported = false;
                break;
            }
        }
        return supported;
	}
	bool VulkanPhysicalDevice::SupportSurface(Surface* surface) {
        VulkanSurface& vulkanSurface = surface->API_VULKAN();
        VkSurfaceKHR vkSurface = vulkanSurface.GetSurface();

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        for (int familyIndex = 0; familyIndex < queueFamilyCount; familyIndex++) {
            if (FamilySupportPresent(familyIndex, vulkanSurface))
                return true;
        }
        return false;
	}
    bool VulkanPhysicalDevice::FamilySupportPresent(uint32_t familyIndex, const VulkanSurface& surface) {
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, familyIndex, surface.GetSurface(), &presentSupport);
        return presentSupport == VK_TRUE;
    }
    PhysicalDeviceQueueInfos VulkanPhysicalDevice::GetFamilyQueueIndices(Surface* surface)
    {
        PhysicalDeviceQueueInfos queueInfos{};
        VulkanSurface& vulkanSurface = surface->API_VULKAN();

        std::unordered_map<QueueType, uint32_t> familyIndices;
        std::unordered_map<uint32_t, uint32_t> familyQueueCount;

        bool hasFoundBestForComputeGraphicPresentFamily = false;

        for (uint32_t familyIndex = 0; familyIndex < queueFamilies.size(); familyIndex++) {
            VkQueueFamilyProperties& queueProperty = queueFamilies[familyIndex];
            if (queueProperty.queueCount < 3) {
                continue;
            }
            if (queueProperty.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT)) {
                
                queueInfos.graphicQueue = QueueInfo(QueueType::GraphicQueue, familyIndex);
                queueInfos.computeQueue = QueueInfo(QueueType::ComputeQueue, familyIndex);
                break;
            }
        }
       
        if (!hasFoundBestForComputeGraphicPresentFamily) {
            for (uint32_t familyIndex = 0; familyIndex < queueFamilies.size(); familyIndex++) {
                VkQueueFamilyProperties& queueProperty = queueFamilies[familyIndex];
                if (!familyIndices.contains(QueueType::GraphicQueue)) {
                    if (familyQueueCount[familyIndex] < queueProperty.queueCount && queueProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                        familyIndices[QueueType::GraphicQueue] = familyIndex;
                        familyQueueCount[familyIndex]++;
                        queueInfos.graphicQueue = QueueInfo(QueueType::GraphicQueue, familyIndex);
                    }
                }
                if (!familyIndices.contains(QueueType::ComputeQueue)) {
                    if (familyQueueCount[familyIndex] < queueProperty.queueCount && queueProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                        familyIndices[QueueType::ComputeQueue] = familyIndex;
                        familyQueueCount[familyIndex]++;
                        queueInfos.computeQueue = QueueInfo(QueueType::ComputeQueue, familyIndex);
                    }
                }
            }
        }
        // find dedicated transfer queue
        for (uint32_t familyIndex = 0; familyIndex < queueFamilies.size(); familyIndex++) {
            VkQueueFamilyProperties& queueProperty = queueFamilies[familyIndex];
            uint32_t queueLeft = queueProperty.queueCount - familyQueueCount[familyIndex];
            if (queueLeft < 2) {
                continue;
            }
            if (queueProperty.queueFlags & VK_QUEUE_TRANSFER_BIT && !(queueProperty.queueFlags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT))) {
                familyQueueCount[familyIndex] += 2;
                queueInfos.transferQueueCpuToGpu = QueueInfo(QueueType::TransferQueue, familyIndex);
                queueInfos.transferQueueGpuToCpu = QueueInfo(QueueType::TransferQueue, familyIndex);
                break;
            }
        }
        // now find 2 transfers queue in family that doesnt support it
        
        return queueInfos;
    }

   
    VkQueueFlags VulkanPhysicalDevice::RhiQueueTypeToVulkanQueueFlags(QueueType queueType) {
        switch (queueType)
        {
        case QueueType::GraphicQueue:
            return VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT;
        case QueueType::ComputeQueue:
            return VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT;
        case QueueType::TransferQueue:
            return VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT;
        default:
            return VkQueueFlagBits::VK_QUEUE_FLAG_BITS_MAX_ENUM;
        }
    }
    VkPhysicalDeviceProperties VulkanPhysicalDevice::GetLimits() {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        return properties;
    }
}