#include "Graphics/RHIResourcePool.hpp"
#include "engine.hpp"

namespace nino_engine {
	template <typename ResourceClass, ERHIResourceType ResourceType, typename ResourceDesc>
	RHIResourcePool<ResourceClass, ResourceType,ResourceDesc>::RHIResourcePool() {
		RendererController* rendererController = GEngine->GetRendererController();
		rendererController->OnPostBeginFrame.Add(this, &RHIResourcePool::OnBeginFrame_RenderThread);
		resourceLifetimeTracker.OnResourceDeleted.Add(this, &RHIResourcePool::OnResourceDeleted);
	}

	template <typename ResourceClass, ERHIResourceType ResourceType, typename ResourceDesc>
	void RHIResourcePool<ResourceClass, ResourceType, ResourceDesc>::OnBeginFrame_RenderThread() {
		resourceLifetimeTracker.UpdateResourcesState();
	}
	
}