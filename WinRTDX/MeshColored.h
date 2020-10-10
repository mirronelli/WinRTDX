#pragma once
#include "pch.h"
#include "Drawable.h"
#include "Structures.h"

namespace Dx::Drawables
{

	class MeshColored : public Drawable
	{
	public:
		MeshColored(
			std::string name,
			std::shared_ptr<Dx::Attachables::VertexBuffer<VertexSimpleWithNormal>> vertexBuffer,
			std::shared_ptr<Dx::Attachables::IndexBuffer> indexBuffer,
			std::shared_ptr<Dx::Attachables::VertexShader> vertexShader,
			std::shared_ptr<Dx::Attachables::PixelShader> pixelShader
		);
		
		void Color(DirectX::XMFLOAT4 color);
		void Specular(float reflectiveness, float reflectionPower);

	protected:
		void RegisterResources();
		void UpdateConstants(DirectX::CXMMATRIX worldTransform);

	private:
		ColorSpecular									mPixelPerInstanceConstants;
		WorldTransform									mVertexPerInstanceConstants;
		std::string										mName;

		static inline int id;
	};

}
