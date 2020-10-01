#pragma once
#include "Scene.h"

namespace Dx::Levels
{

	class SceneFactory
	{
		static std::unique_ptr<Dx::Drawables::Scene> LoadFromFile(std::string fileName);
	};

}
