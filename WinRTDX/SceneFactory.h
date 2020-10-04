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
			std::string fileName, 
			std::shared_ptr<Dx::Attachables::VertexShader> vertexShader, 
			std::shared_ptr<Dx::Attachables::PixelShader> pixelShader);

	private:
		static void SceneFactory::LoadMeshesToScene(
			Dx::Drawables::Scene* parentScene, 
			aiNode* node, 
			DirectX::CXMMATRIX parentMatrix, 
			const aiScene* sourceScene, 
			std::shared_ptr<Dx::Attachables::VertexShader> vertexShader, 
			std::shared_ptr<Dx::Attachables::PixelShader> pixelShader);
		
		static void SceneFactory::CopyMeshesToScene(
			Dx::Drawables::Scene* parentScene, 
			aiNode* node, 
			const aiScene* sourceScene, 
			std::shared_ptr<Dx::Attachables::VertexShader> vertexShader, 
			std::shared_ptr<Dx::Attachables::PixelShader> pixelShader);
	};

}
