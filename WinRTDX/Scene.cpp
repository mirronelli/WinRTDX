#include "pch.h"
#include "Scene.h"

using namespace DirectX;

namespace Dx::Drawables
{

	Scene::Scene() {}

	void Scene::AddDrawable(std::unique_ptr<Drawable> drawable)
	{
		mDrawables.push_back(std::move(drawable));
	}

	void Scene::AddScene(std::unique_ptr<Scene> scene)
	{
		mScenes.push_back(std::move(scene));
	}

	void Scene::Draw()
	{
		for (auto& scene : mScenes)
			scene->Draw();

		for (auto& drawable : mDrawables)
			drawable->Draw();
	}

	void Scene::Update(float delta, CXMMATRIX parentMatrix)
	{
		ObjectInSpace::Update(delta, parentMatrix);

		for (auto& scene : mScenes)
			scene->Update(delta, mTransform);

		for (auto& drawable : mDrawables)
			drawable->Update(delta, mTransform);
	}
}