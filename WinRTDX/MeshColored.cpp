#include "pch.h"
#include "MeshColored.h"


namespace Dx::Drawables
{
	MeshColored::MeshColored(std::string name, std::shared_ptr<Dx::Attachables::VertexBuffer<VertexSimpleWithNormal>> vertexBuffer, std::shared_ptr<Dx::Attachables::IndexBuffer> indexBuffer, std::shared_ptr<Dx::Attachables::VertexShader> vertexShader, std::shared_ptr<Dx::Attachables::PixelShader> pixelShader)
	: 
		mName(name), Drawable(vertexShader, pixelShader)
	{
		mVertexBuffer = vertexBuffer;
		mIndexBuffer = indexBuffer;
		mInputLayout =		InputLayout										::Get(VertexType::SimpleWithNormal);
		mVsConstantBuffer = VSConstantBuffer<WorldTransform>		::Create(mVertexPerInstanceConstants, ResourceSlots::PerInstance);
		mPsConstantBuffer = PSConstantBuffer<ColorSpecular>		::Create(mPixelPerInstanceConstants, ResourceSlots::PerInstance);
		mIndicesCount = mIndexBuffer->Count();
	}

	void MeshColored::Color(DirectX::XMFLOAT4 color)
	{
		mPixelPerInstanceConstants.color = color;
	}

	void MeshColored::Specular(float reflectiveness, float reflectionPower)
	{
		mPixelPerInstanceConstants.reflectiveness = reflectiveness;
		mPixelPerInstanceConstants.reflectionPower = reflectionPower;
	}

	void MeshColored::RegisterResources(){}

	void MeshColored::UpdateConstants(DirectX::CXMMATRIX worldTransform)
	{
		mVertexPerInstanceConstants.worldTransform = worldTransform;

		std::static_pointer_cast<VSConstantBuffer<WorldTransform>>		(mVsConstantBuffer)->Update(mVertexPerInstanceConstants);
		std::static_pointer_cast<PSConstantBuffer<ColorSpecular>>		(mPsConstantBuffer)->Update(mPixelPerInstanceConstants);
	}
}