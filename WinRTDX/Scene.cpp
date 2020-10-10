#include "pch.h"
#include "Scene.h"
#include "Mesh.h"

using namespace DirectX;

namespace Dx::Drawables
{
	Scene::Scene(std::string name) : mName(name) {}

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

	std::string Scene::Name()
	{
		return mName;
	}

	std::unique_ptr<Scene> Scene::Clone()
	{
		std::string cloneName = MAKEID(mName, mInstanceIndexes[mName]++);
		std::unique_ptr clone = std::make_unique<Scene>(cloneName);
		clone->mInitialQuaternion = this->mInitialQuaternion;
		clone->mTransform = this->mTransform;
		clone->mRotationSpeedX = this->mRotationSpeedX;
		clone->mRotationSpeedY = this->mRotationSpeedY;
		clone->mRotationSpeedZ = this->mRotationSpeedZ;
		clone->mSpeedX = this->mSpeedX;
		clone->mSpeedY = this->mSpeedY;
		clone->mSpeedZ = this->mSpeedZ;
		clone->mRotationX = this->mRotationX;
		clone->mRotationY = this->mRotationY;
		clone->mRotationZ = this->mRotationZ;
		clone->mScaleX = this->mScaleX;
		clone->mScaleY = this->mScaleY;
		clone->mScaleZ = this->mScaleZ;
		clone->mX = this->mX;
		clone->mY = this->mY;
		clone->mZ = this->mZ;

		for (int i = 0; i < mDrawables.size(); i++)
		{
			std::unique_ptr<Drawable> drawableClone = mDrawables[i]->Clone();
			if (drawableClone != nullptr)
				clone->AddDrawable(std::move(drawableClone));
		}

		for (int i = 0; i < mScenes.size(); i++)
		{
			std::unique_ptr<Scene> sceneClone = mScenes[i]->Clone();
			clone->AddScene(std::move(sceneClone));
		}

		return clone;
	}
}