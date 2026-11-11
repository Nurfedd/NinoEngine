#pragma once

#include "Interface/rhi_resource.hpp"
#include "Interface/command_buffer.hpp"
#include <vector>
#include <memory>
namespace rhi
{
	class DeviceQueue;
	class VulkanCommandPool;

	struct CommandPoolDesc
	{
	public :
		CommandPoolDesc() = default;
		CommandPoolDesc(DeviceQueue* inQueue) :
			Queue(inQueue)
		{
		}

	public :
		DeviceQueue* Queue = nullptr;
	};

	class CommandPool : public RHIResource
	{
	public :
		CommandPool(Device* inDevice, const CommandPoolDesc& inDesc);
		~CommandPool() override = default;

		virtual std::vector<CommandBuffer*> AllocateCommandBuffers(uint32_t inCount, bool inIsPrimary = true) = 0;
		virtual VulkanCommandPool& API_VULKAN();

	protected :
		std::vector<std::unique_ptr<CommandBuffer>> allocatedCommandBuffers;
		CommandPoolDesc desc;
		DeviceQueue* queue = nullptr;
	};
}
