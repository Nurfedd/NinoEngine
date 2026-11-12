#pragma once

namespace rhi
{
	class Device;
	enum class RHIResourceType
	{
		RT_Buffer,
		RT_Texture,
	};
	class RHIResource
	{
	public :
		virtual ~RHIResource() = default;

		RHIResource(const RHIResource&) = delete;
		RHIResource& operator=(const RHIResource&) = delete;
		RHIResource(RHIResource&&) = delete;
		RHIResource& operator=(RHIResource&&) = delete;

		Device* GetDevice() const
		{
			return device;
		}

	protected :
		explicit RHIResource(Device* inDevice) : device(inDevice) {}

	protected :
		Device* device;
	};
}

