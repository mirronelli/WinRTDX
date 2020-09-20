#pragma once
#include <typeindex>
#include <map>

namespace Dx {

	class ResourceManager
	{
	public:
		static void* GetResource(std::type_index type, uint16_t id)
		{
			void* result = nullptr;
			std::map<uint16_t, void*>* innerMap;
			innerMap = typeInstanceMaps[type];
			if (innerMap != nullptr)
				result = (*innerMap)[id];

			return result;
		}

		static void SetResource(std::type_index type, uint16_t id, void* resource)
		{
			std::map<uint16_t, void*>* innerMap;
			innerMap = typeInstanceMaps[type];
			if (innerMap == nullptr)
			{
				innerMap = new std::map<uint16_t, void*>;
				typeInstanceMaps[type] = innerMap;
			}
			(*innerMap)[id] =resource;
		}

		static uint16_t GetCurrentInstance(std::type_index type)
		{
			return current_instances[type];
		}

		static void SetCurrentInstance(std::type_index type, uint16_t id)
		{
			current_instances[type] = id;
		}
	private:
		inline static std::map<std::type_index, std::map< uint16_t, void* >* > typeInstanceMaps;
		inline static std::map<std::type_index, uint16_t > current_instances;
	};

}
