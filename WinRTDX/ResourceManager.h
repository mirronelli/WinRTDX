#pragma once
#include "pch.h"
#include <map>

#include "Structures.h"
#include "InputLayout.h"

namespace Dx::Attachables {
	enum class ResourceSlots
	{
		PerLevel = 0,
		PerFrame = 1,
		PerInstance = 2
	};

	class ResourceManager
	{
	public:
		static void ClearCache();

		static std::shared_ptr<Dx::Attachables::InputLayout> GetInputLayout(Dx::Drawables::VertexType type);

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

		static Dx::Drawables::VertexType CurrentInputLayout();
		static void CurrentInputLayout(Dx::Drawables::VertexType);

	private:
		inline static Dx::Drawables::VertexType mCurrentInputLayout;
		inline static std::map <Dx::Drawables::VertexType, std::shared_ptr<Dx::Attachables::InputLayout>>	InputLayouts;
	};
}