#pragma once

#include "Drawable.h"
#include "ObjectInSpace.h"

namespace Dx::Drawables
{
	class Scene : public ObjectInSpace
	{
	public:
		Scene();
		void AddDrawable(std::unique_ptr<Drawable> drawable);
		void AddScene(std::unique_ptr<Scene> drawable);
		void Draw();
		void Update(float delta, DirectX::CXMMATRIX parentMatrix);

	private:
		std::vector<std::unique_ptr<Scene>>			mScenes;
		std::vector<std::unique_ptr<Drawable>>		mDrawables;
	};
}