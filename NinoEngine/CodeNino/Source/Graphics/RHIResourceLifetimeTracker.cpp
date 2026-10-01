#include "Graphics/RHIResourceLifetimeTracker.hpp"
#include "engine.hpp"

namespace nino_engine {
	bool RHIResourceLifetimeTracker::IsResourceInUse(IRHIResource* Resource) {
		ResourcePoolStateInfo stateInfo;
		if (GetResourcePoolStateInfo(Resource, stateInfo)) {
			return stateInfo.flags & RPS_Freed;
		}
		return false;
	}

	void RHIResourceLifetimeTracker::UpdateResourcesState() {
		for (auto pair : activeResources) {
			HandleResourcePoolState(pair.first, pair.second);
		}
	}
	void RHIResourceLifetimeTracker::HandleResourcePoolState(IRHIResource* Resource, ResourcePoolStateInfo& ResourcePoolState) {
		EngineGraphicResources* graphicResource = GEngine->EngineServices.Get<EngineGraphicResources>();

		RendererController* rendererController = GEngine->GetRendererController();
		uint64_t currentFrame = rendererController->GetFrameCount();

		if (ResourcePoolState.flags & ResourcePoolStateFlags::RPS_BeingFreed) {
			if (currentFrame >= ResourcePoolState.freedFrameIndex) {
				ResourcePoolState.flags |= RPS_Freed;
				RemoveFlag<ResourcePoolStateFlags>(ResourcePoolState.flags, RPS_BeingFreed);
			}
		}
		if (ResourcePoolState.flags & (ResourcePoolStateFlags::RPS_Freed | ResourcePoolStateFlags::RPS_DestroyOnFreed)) {
			if (currentFrame == ResourcePoolState.destroyAfterFrameIndex) {
				Resource->Destroy(graphicResource->Device);
				activeResources.erase(Resource);
				OnResourceDeleted.Broadcast(Resource);
			}
		}
	}

	bool RHIResourceLifetimeTracker::GetResourcePoolStateInfo(IRHIResource* Resource, ResourcePoolStateInfo& ResourcePoolStateInfo) {
		if (Resource == nullptr) {
			return false;
		}
		auto it = activeResources.find(Resource);
		if (it == activeResources.end()) {
			return false;
		}
		ResourcePoolStateInfo = it->second;
		return true;
	}

	void RHIResourceLifetimeTracker::FreeResource(IRHIResource* Resource, uint32_t freedFrameCount) {

		ResourcePoolStateInfo stateInfo;
		RendererController* rendererController = GEngine->GetRendererController();
		bool resourceExists = GetResourcePoolStateInfo(Resource, stateInfo);

		stateInfo.flags = RPS_BeingFreed;
		stateInfo.freedFrameIndex = rendererController->GetFrameCount() + freedFrameCount;
		stateInfo.destroyAfterFrameIndex = 0;

		if (!resourceExists) {
			activeResources[Resource] = stateInfo;
		}

	}
	void RHIResourceLifetimeTracker::FreeResourceBasedOnSwapchainImageCount(IRHIResource* Resource) {
		EngineGraphicResources* graphicResource = GEngine->EngineServices.Get<EngineGraphicResources>();
		uint32_t frameInFlightCount = graphicResource->GetFrameInFlightCount();
		FreeResource(Resource, frameInFlightCount);
	}

	void RHIResourceLifetimeTracker::FreeResourceAndDestroy(IRHIResource* Resource, uint32_t freedFrameCount) {
		FreeResourceAndDestroyAfterFrameCount(Resource, freedFrameCount, 0);
	}

	void RHIResourceLifetimeTracker::FreeResourceAndDestroyAfterFrameCount(IRHIResource* Resource, uint32_t freedFrameCount, uint32_t destroyFrameCount) {
		ResourcePoolStateInfo stateInfo;
		RendererController* rendererController = GEngine->GetRendererController();
		bool resourceExists = GetResourcePoolStateInfo(Resource, stateInfo);

		stateInfo.flags = RPS_BeingFreed | RPS_DestroyOnFreed;
		stateInfo.freedFrameIndex = rendererController->GetFrameCount();
		stateInfo.destroyAfterFrameIndex = rendererController->GetFrameCount() + destroyFrameCount;

		if (!resourceExists) {
			activeResources[Resource] = stateInfo;
		}
	}
}