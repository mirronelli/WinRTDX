#pragma once
#include "pch.h"
#include "Drawable.h"
#include "Structures.h"

namespace Dx::Drawables
{

	class Mesh : public Drawable
	{
	public:
		Mesh(
			std::string name,
			std::shared_ptr<InputLayout> inputLayout,
			std::shared_ptr<Dx::Attachables::VertexBufferBase> vertexBuffer,
			std::shared_ptr<Dx::Attachables::IndexBuffer> indexBuffer,
			std::shared_ptr<Dx::Attachables::VertexShader> vertexShader,
			std::shared_ptr<Dx::Attachables::PixelShader> pixelShader,
			std::shared_ptr<Dx::Attachables::Texture> texture
		);
		
		void Color(DirectX::XMFLOAT4 color);
		void Specular(float reflectiveness, float reflectionPower);
		std::unique_ptr<Dx::Drawables::Drawable> Clone();

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
