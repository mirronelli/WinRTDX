#pragma once
#include "Scene.h"
#include "assimp\scene.h"
#include "Attachable.h"

namespace Dx::Levels
{

	class SceneFactory
	{
	public:
		static std::unique_ptr<Dx::Drawables::Scene> LoadFromFile(
			std::string fileName
		);

	private:
		static void SceneFactory::LoadMeshesToScene(
			Dx::Drawables::Scene* parentScene, 
			aiNode* node, 
			const aiScene* sourceScene
		);
		
		static void SceneFactory::CopyMeshesToScene(
			Dx::Drawables::Scene* parentScene, 
			aiNode* node, 
			const aiScene* sourceScene
		);

		static DirectX::XMMATRIX ConvertMatrix(aiMatrix4x4& source);
	};

}
