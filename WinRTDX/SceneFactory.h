#pragma once
#include "Scene.h"
#include "assimp\scene.h"
#include "Attachable.h"
#include "Mesh.h"
#include <map>

namespace Dx::Levels
{
	class SceneFactory
	{
	public:
		static std::unique_ptr<Dx::Drawables::Scene> Create(
			std::string fileName
		);

	private:
		static void SceneFactory::ProcessNode(
			Dx::Drawables::Scene* parentScene, 
			aiNode* node, 
			const aiScene* sourceScene,
			std::string baseName,
			std::string folderName
		);
		
		static void SceneFactory::CopyMeshesToScene(
			Dx::Drawables::Scene* parentScene, 
			aiNode* node, 
			const aiScene* sourceScene,
			std::string baseName,
			std::string folderName
		);

		static std::unique_ptr<Dx::Drawables::Mesh> CreateMeshColored(aiMesh* sourceMesh, const aiScene* sourceScene, std::string baseName, std::string folderName);
		static DirectX::XMMATRIX ConvertMatrix(aiMatrix4x4& source);
		static inline std::map<std::string, std::shared_ptr<Dx::Drawables::Scene>> mScenes;
	};
}
