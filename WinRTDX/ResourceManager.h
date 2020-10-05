#pragma once
#include "pch.h"
#include <map>

#include "Structures.h"
#include "InputLayout.h"

namespace Dx::Attachables {
	class ResourceManager
	{
	public:
		static void ClearCache();
		inline static int CurrentTexture = 0;
		inline static std::map < int, std::shared_ptr<void>>	Textures;
	};
}