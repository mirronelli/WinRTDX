#pragma once
#include "Scene.h"

namespace Dx::Levels
{

	class SceneFactory
	{
	public:
		static std::unique_ptr<Dx::Drawables::Scene> LoadFromFile(std::string fileName, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader);
	};

}
