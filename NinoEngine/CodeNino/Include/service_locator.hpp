#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>
#include "service.hpp"
#include <type_traits>

namespace nino_engine {
	
	template<typename ServiceBase>
	
	class ServiceLocator {
	public :
		static_assert(std::is_base_of<Service, ServiceBase>::value);
		template <typename T, typename... Args>
		T* RegisterService(Args&&... args)
		{
			static_assert(std::is_base_of<ServiceBase, T>::value);
			T* currentService = Exists<T>();
			if (currentService != nullptr) return currentService;

			std::type_index typeIndex = typeid(T);
			std::unique_ptr<T> newService = std::make_unique<T>(std::forward(args));
			T* rawPtr = newService.get();
			services[typeIndex] = std::move(newService);

			return rawPtr;
		}
		template <typename T>
		T* Get() 
		{
			static_assert(std::is_base_of<ServiceBase, T>::value);
			std::type_index typeIndex = typeid(T);
			return dynamic_cast<T*>(services[typeIndex].get());
		}
		template <typename T>
		bool Exists() 
		{
			static_assert(std::is_base_of<ServiceBase, T>::value);
			return Get<T>() != nullptr;
		}
		template <typename T>
		void UnregisterService() 
		{
			static_assert(std::is_base_of<ServiceBase, T>::value);
			std::type_index typeIndex = typeid(T);
			T* service = Get<T>();
			if (service) {
				services.erase(typeIndex);
			}
		}
		void Clear() 
		{
			for (auto& pair : services) {
				std::unique_ptr<Service>& service = pair.second;
				service->OnDestroy();
			}
			for (auto& pair : services) {
				std::unique_ptr<Service>& service = pair.second;
				service->OnLateDestroy();
			}
			services.clear();
		};
	private :
		std::unordered_map<std::type_index, std::unique_ptr<Service>> services;
		
	};
}