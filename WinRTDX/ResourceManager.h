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

		inline static uint16_t CurrentInputLayout = 0;
		inline static uint16_t CurrentVertexBuffer = 0;
		inline static uint16_t CurrentIndexBuffer = 0;

		inline static uint16_t CurrentPSConstantBuffer = 0;
		inline static uint16_t CurrentVSConstantBuffer = 0;

		inline static uint16_t CurrentPixelShader = 0;
		inline static uint16_t CurrentVertexShader = 0;


	private:
		inline static std::map<std::type_index, std::map< uint16_t, void* >* > typeInstanceMaps;
	};

}
