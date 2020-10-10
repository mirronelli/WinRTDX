#pragma once

#include "Drawable.h"
#include "ObjectInSpace.h"

namespace Dx::Drawables
{
	class Scene : public ObjectInSpace
	{
	public:
		Scene(std::string name = std::string(""));
		void AddDrawable(std::unique_ptr<Drawable> drawable);
		void AddScene(std::unique_ptr<Scene> drawable);
		void Draw();
		void Update(float delta, DirectX::CXMMATRIX parentMatrix);
		std::string Name();
		std::unique_ptr<Scene> Clone();

	private:
		std::vector<std::unique_ptr<Scene>>			mScenes;
		std::vector<std::unique_ptr<Drawable>>		mDrawables;
		std::string											mName;
		static inline std::map<std::string, int>	mInstanceIndexes;
	};
}