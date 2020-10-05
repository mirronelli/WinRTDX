#include <pch.h>
#include "ResourceManager.h"
#include "Structures.h"

namespace Dx::Attachables
{

	void ResourceManager::ClearCache()
	{
		CurrentTexture = 0;
		Textures.clear();
	}
}