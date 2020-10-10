#pragma once
#include "Scene.h"
#include "assimp\scene.h"
#include "Attachable.h"
#include "MeshColored.h"

namespace Dx::Levels
{
	class SceneFactory
	{
	public:
		static std::unique_ptr<Dx::Drawables::Scene> LoadFromFile(
			std::string fileName
		);

	private:
		static void SceneFactory::ProcessNode(
			Dx::Drawables::Scene* parentScene, 
			aiNode* node, 
			const aiScene* sourceScene,
			std::string baseName
		);
		
		static void SceneFactory::CopyMeshesToScene(
			Dx::Drawables::Scene* parentScene, 
			aiNode* node, 
			const aiScene* sourceScene,
			std::string baseName
		);

		static std::unique_ptr<Dx::Drawables::MeshColored> CreateMeshColored(
			aiMesh* sourceMesh, 
			std::string baseName
		);

		static DirectX::XMMATRIX ConvertMatrix(aiMatrix4x4& source);
	};
}
