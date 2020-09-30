#pragma once
#include <memory.h>
#include <DirectXMath.h>

#include "Drawable.h"
#include "ObjectInSpace.h"

namespace Dx::Drawables
{
	class Scene : ObjectInSpace
	{
	public:
		Scene();
		void Draw();
		void Update(float delta, DirectX::CXMMATRIX parentMatrix);

	private:
		std::vector<std::shared_ptr<Scene>>			mChildScenes;
		std::vector<std::shared_ptr<Drawable>>		mDrawables;
	};
}