#pragma once
#include "RHIResources/RHIResource.hpp"
#include "Graphics/RHIResourceLifetimeTracker.hpp"
#include "Graphics/engine_graphic_resources.hpp"
#include "nino_core.hpp"
#include "engine.hpp"
#include <unordered_map>
#include <memory>
namespace nino_engine {

	template <typename ResourceClass,ERHIResourceType ResourceType,typename ResourceDesc>
	class RHIResourcePool {
		static_assert(std::is_base_of_v<IRHIResource, ResourceClass>,"ResourceClass must inherit from IRHIResource");
	public :
		RHIResourcePool();
		
	private :
		RHIResourceLifetimeTracker resourceLifetimeTracker;
		std::unordered_map<ResourceDesc, std::vector<std::unique_ptr<ResourceClass>>> activeResourcesDesc;

		ResourceClass* AllocateResource(ResourceDesc resourceDesc) {
			auto it = activeResourcesDesc.find(resourceDesc);
			if (it != activeResourcesDesc.end()) { // Get resource from pool
				std::vector<std::unique_ptr<ResourceClass>>& activeResourceArray = it->second;
				for (std::unique_ptr<ResourceClass>& activeResource : activeResourceArray) {
					if (!resourceLifetimeTracker.IsResourceInUse(activeResource.get())) {
						return activeResource.get();
					}
				}
			}
			EngineGraphicResources* graphicsResources = GEngine->EngineServices.Get<EngineGraphicResources>();
			std::unique_ptr<ResourceClass> newResource = std::make_unique<ResourceClass>(ResourceType,resourceDesc,graphicsResources->Device);
			
			std::vector<std::unique_ptr<ResourceClass>> newResourcesArray;
			newResourcesArray.push_back(std::move(newResource));
			activeResourcesDesc[resourceDesc] = newResourcesArray;
		}

		void OnResourceDeleted(IRHIResource* DeletedResource) {
			ResourceClass* resource = dynamic_cast<ResourceClass*>(DeletedResource);
			if (!resource) {
				return;
			}
			ResourceDesc resourceDesc = resource->GetDesc();
			auto it = activeResourcesDesc.find(resource);
			if (it == activeResourcesDesc.end()) {
				return;
			}
			std::vector<std::unique_ptr<ResourceClass>>& activeResourceArray = it->second;
			std::ranges::remove_if(activeResourceArray, [resource](std::unique_ptr<ResourceClass>& activeResource) {
				return activeResource.get() == resource;
				});
		}
		void OnBeginFrame_RenderThread();
	};
}
