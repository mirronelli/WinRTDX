#include <pch.h>
#include "ResourceManager.h"
#include "Structures.h"

namespace Dx::Attachables
{

	void ResourceManager::ClearCache()
	{
		mCurrentInputLayout = Dx::Drawables::VertexType::None;
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

	std::shared_ptr<InputLayout> ResourceManager::GetInputLayout(Dx::Drawables::VertexType type)
	{
		std::shared_ptr<InputLayout> result = InputLayouts[type];
		if (!result)
		{
			result = std::make_shared<InputLayout>(type);
			InputLayouts[type] = result;
		}
		return result;
	}

	Dx::Drawables::VertexType ResourceManager::CurrentInputLayout()
	{
		return mCurrentInputLayout;
	}

	void ResourceManager::CurrentInputLayout(Dx::Drawables::VertexType value)
	{
		mCurrentInputLayout = value;
	}
}