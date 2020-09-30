#include "pch.h"
#include "Scene.h"

using namespace DirectX;
using namespace Dx::Drawables;

Scene::Scene()
{

}

void Scene::Draw()
{
	for (auto scene : mChildScenes)
		Draw();

	for (auto drawable : mDrawables)
		Draw();
}

void Scene::Update(float delta, CXMMATRIX parentMatrix)
{
	ObjectInSpace::Update(delta, parentMatrix);
	for (auto scene : mChildScenes)
		Update(delta, mTransform);

	for (auto drawable : mDrawables)
		Update(delta, mTransform);
}