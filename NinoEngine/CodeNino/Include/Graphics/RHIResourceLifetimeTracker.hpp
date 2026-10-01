#pragma once
#include "RHIResources/RHIResource.hpp"
#include "nino_core.hpp"
#include <unordered_map>
#include <memory>


namespace nino_engine {
	DECLARE_DELEGATE(DResourceDeleted,IRHIResource*)

	class RHIResourceLifetimeTracker {
	public:
		void UpdateResourcesState();
		bool IsResourceInUse(IRHIResource* Resource);
		void FreeResource(IRHIResource* Resource, uint32_t freedFrameCount);
		void FreeResourceBasedOnSwapchainImageCount(IRHIResource* Resource);
		void FreeResourceAndDestroy(IRHIResource* Resource, uint32_t freedFrameCount);
		// free resource and then try to destroy it after frameCount frames if the resource has not been allocated during that time
		void FreeResourceAndDestroyAfterFrameCount(IRHIResource* Resource, uint32_t freedFrameCount, uint32_t destroyFrameCount);
	
		DResourceDeleted OnResourceDeleted;
	private:
		enum ResourcePoolStateFlags {
			RPS_InUse,
			RPS_BeingFreed,
			RPS_Freed,
			RPS_DestroyOnFreed
		};
		ENUM_CLASS_FRIEND_FLAGS(ResourcePoolStateFlags)
		struct ResourcePoolStateInfo {
			uint64_t freedFrameIndex;
			ResourcePoolStateFlags flags;
			uint32_t destroyAfterFrameIndex;
		};

		void HandleResourcePoolState(IRHIResource* Resource, ResourcePoolStateInfo& ResourcePoolState);
		bool GetResourcePoolStateInfo(IRHIResource* Resource, ResourcePoolStateInfo& ResourcePoolStateInfo);
		std::unordered_map<IRHIResource*, ResourcePoolStateInfo> activeResources;
	};
	
}
