#include "IVulkan/vulkan_device_queue.hpp"
#include "IVulkan/vulkan_device.hpp"
#include "IVulkan/vulkan_fence.hpp"
#include "IVulkan/vulkan_semaphore.hpp"
#include "IVulkan/vulkan_command_buffer.hpp"
#include "IVulkan/enums/vulkan_synchronization_stage.hpp"
#include "Volk/volk.h"

namespace rhi {
	
	VulkanDeviceQueue::VulkanDeviceQueue(VkDevice vulkanDevice, uint32_t _familyIndex) {
		familyIndex = _familyIndex;
		vkGetDeviceQueue(vulkanDevice, familyIndex, 0, &queue);
		
	}
	std::vector<VkSemaphoreSubmitInfo> VulkanDeviceQueue::GetVulkanSubmitInfo(const SemaphoreSubmitInfo& submitInfo) 
	{
		std::vector<VkSemaphoreSubmitInfo> semaphoreSubmitInfo (submitInfo.semaphoreCount);
		
		uint32_t binaryIndex = semaphoreSubmitInfo.size() - 1;
		uint32_t timelineIndex = 0;

		for (uint32_t i = 0; i < semaphoreSubmitInfo.size(); i++) 
		{
			VulkanSemaphore& vulkanSemaphore = submitInfo.semaphores[i]->API_VULKAN();
			semaphoreSubmitInfo[i].sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;

			if (vulkanSemaphore.IsBinary()) 
			{
				semaphoreSubmitInfo[binaryIndex].semaphore = vulkanSemaphore.GetSemaphore();
				semaphoreSubmitInfo[binaryIndex].value = 0;
				semaphoreSubmitInfo[binaryIndex].stageMask = ToVulkanPipelineStage2(submitInfo.stageFlags[i]);
				binaryIndex--;
			}
			else {
				semaphoreSubmitInfo[timelineIndex].semaphore = vulkanSemaphore.GetSemaphore();
				semaphoreSubmitInfo[timelineIndex].value = submitInfo.timelineValues[timelineIndex];
				semaphoreSubmitInfo[timelineIndex].stageMask = ToVulkanPipelineStage2(submitInfo.stageFlags[i]);
				timelineIndex++;
			}
		}
		return semaphoreSubmitInfo;
	}
	

	void VulkanDeviceQueue::Submit(CommandBuffer** commandsBuffer, uint32_t commandBufferCount, SubmitInfo submitInfo, Fence* fence) 
	{
		
		SemaphoreSubmitInfo waitSemaphoreInfo = submitInfo.waitSemaphoreInfo;
		SemaphoreSubmitInfo signalSemaphoreInfo = submitInfo.signalSemaphoreInfo;

		VkSubmitInfo2 queueSubmitInfo{};
		queueSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
		VkFence vkFence = fence ? fence->API_VULKAN().GetFence() : VK_NULL_HANDLE;

		std::vector<VkCommandBufferSubmitInfo> commandBufferInfo(commandBufferCount);
		std::vector<VulkanCommandBuffer> vulkanCommandBuffers(commandBufferCount);

		for (uint32_t i = 0; i < commandBufferCount; i++) 
		{
			vulkanCommandBuffers[i] = commandsBuffer[i]->API_VULKAN();
			commandBufferInfo[i].sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
			commandBufferInfo[i].commandBuffer = commandsBuffer[i]->API_VULKAN().commandBuffer;
		}

		std::vector<VkSemaphoreSubmitInfo> waitInfo = GetVulkanSubmitInfo(submitInfo.waitSemaphoreInfo);
		std::vector<VkSemaphoreSubmitInfo> signalInfo = GetVulkanSubmitInfo(submitInfo.signalSemaphoreInfo);

		queueSubmitInfo.pCommandBufferInfos = commandBufferInfo.data();
		queueSubmitInfo.commandBufferInfoCount = commandBufferInfo.size();

		queueSubmitInfo.pWaitSemaphoreInfos = waitInfo.data();
		queueSubmitInfo.waitSemaphoreInfoCount = waitInfo.size();

		queueSubmitInfo.pSignalSemaphoreInfos = signalInfo.data();
		queueSubmitInfo.signalSemaphoreInfoCount = signalInfo.size();

		VkResult result = vkQueueSubmit2(queue, 1, &queueSubmitInfo, vkFence);
	}

	void VulkanDeviceQueue::Submit(CommandBuffer** commandsBuffer, uint32_t commandBufferCount, Fence* fence)
	{
		SubmitInfo submitInfo{};
		submitInfo.signalSemaphoreInfo.semaphoreCount = 0;
		submitInfo.waitSemaphoreInfo.semaphoreCount = 0;
		Submit(commandsBuffer, commandBufferCount, submitInfo, fence);
	}

	bool VulkanDeviceQueue::Present(PresentInfo presentInfo) 
	{
		std::vector<VkSemaphore> waitSemaphores(presentInfo.waitSemaphoreCount);

		for (uint32_t i = 0; i < waitSemaphores.size(); i++) 
		{
			VulkanSemaphore& vulkanSemaphore = presentInfo.waitSemaphores[i]->API_VULKAN();
			if (!vulkanSemaphore.IsBinary()) 
			{
				throw std::exception("Sempahore for present must be binary");
				return false;
			}
			waitSemaphores[i] = vulkanSemaphore.GetSemaphore();
		}

		VulkanSwapchain& vulkanSwapchain = presentInfo.swapchain->API_VULKAN();
		uint32_t imageIndex = vulkanSwapchain.GetImageIndex();
		VkSwapchainKHR swapchain = vulkanSwapchain.GetSwapchain();

		VkPresentInfoKHR vkPresentInfo{};
		vkPresentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		vkPresentInfo.pImageIndices = &imageIndex;
		vkPresentInfo.pSwapchains = &swapchain;
		vkPresentInfo.swapchainCount = 1;
		vkPresentInfo.pWaitSemaphores = waitSemaphores.data();
		vkPresentInfo.waitSemaphoreCount = waitSemaphores.size();

		VkResult result = vkQueuePresentKHR(queue, &vkPresentInfo);
		return (result == VK_SUCCESS);
	}
}