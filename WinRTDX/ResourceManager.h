#pragma once
#include "pch.h"
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

			CurrentTexture = 0;
			Textures.clear();
		}

		inline static int CurrentInputLayout = 0;
		inline static std::map < int, std::shared_ptr<void>>	InputLayouts;

		inline static int CurrentVertexBuffer = 0;
		inline static std::map < int, std::shared_ptr<void>>	VertexBuffers;

		inline static int CurrentIndexBuffer = 0;
		inline static std::map < int, std::shared_ptr<void>>	IndexBuffers;

		inline static int CurrentPSConstantBuffer = 0;
		inline static std::map < int, std::shared_ptr<void>>	PSConstantBuffers;

		inline static int CurrentVSConstantBuffer = 0;
		inline static std::map < int, std::shared_ptr<void>>	VSConstantBuffers;

		inline static int CurrentPixelShader = 0;
		inline static std::map < int, std::shared_ptr<void>>	PixelShaders;

		inline static int CurrentVertexShader = 0;
		inline static std::map < int, std::shared_ptr<void>>	VertexShaders;

		inline static int CurrentTexture = 0;
		inline static std::map < int, std::shared_ptr<void>>	Textures;
	};
}