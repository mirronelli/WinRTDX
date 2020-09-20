#pragma once
#include <pch.h>
#include <map>

namespace Dx::Attachables {

	class ResourceManager
	{
	public:
		inline static void ClearCache()
		{
			CurrentInputLayout = 0;
			InputLayouts.clear();

			CurrentVertexBuffer = 0;
			VertexBuffers.clear();

			CurrentIndexBuffer = 0;
			IndexBuffers.clear();

			CurrentPSConstantBuffer = 0;
			PSConstantBuffers.clear();

			CurrentVSConstantBuffer = 0;
			VSConstantBuffers.clear();

			CurrentPixelShader = 0;
			PixelShaders.clear();

			CurrentVertexShader = 0;
			VertexShaders.clear();
		}

		inline static uint16_t CurrentInputLayout = 0;
		inline static std::map < uint16_t, std::shared_ptr<void>>	InputLayouts;

		inline static uint16_t CurrentVertexBuffer = 0;
		inline static std::map < uint16_t, std::shared_ptr<void>>	VertexBuffers;

		inline static uint16_t CurrentIndexBuffer = 0;
		inline static std::map < uint16_t, std::shared_ptr<void>>	IndexBuffers;

		inline static uint16_t CurrentPSConstantBuffer = 0;
		inline static std::map < uint16_t, std::shared_ptr<void>>	PSConstantBuffers;

		inline static uint16_t CurrentVSConstantBuffer = 0;
		inline static std::map < uint16_t, std::shared_ptr<void>>	VSConstantBuffers;

		inline static uint16_t CurrentPixelShader = 0;
		inline static std::map < uint16_t, std::shared_ptr<void>>	PixelShaders;
		
		inline static uint16_t CurrentVertexShader = 0;
		inline static std::map < uint16_t, std::shared_ptr<void>>	VertexShaders;
	};
}