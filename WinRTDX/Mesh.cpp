#include "pch.h"
#include "Mesh.h"


namespace Dx::Drawables
{
	Mesh::Mesh(
		std::string name, 
		std::shared_ptr<InputLayout> inputLayout, 
		std::shared_ptr<Dx::Attachables::Attachable> vertexBuffer, 
		std::shared_ptr<Dx::Attachables::IndexBuffer> indexBuffer, 
		std::shared_ptr<Dx::Attachables::VertexShader> vertexShader, 
		std::shared_ptr<Dx::Attachables::PixelShader> pixelShader
	)
	: 
		mName(name), Drawable(vertexShader, pixelShader)
	{
		mVertexBuffer = vertexBuffer;
		mIndexBuffer = indexBuffer;
		mInputLayout = inputLayout;
		mVsConstantBuffer = VSConstantBuffer<WorldTransform>		::Create(mVertexPerInstanceConstants, ResourceSlots::PerInstance);
		mPsConstantBuffer = PSConstantBuffer<ColorSpecular>		::Create(mPixelPerInstanceConstants, ResourceSlots::PerInstance);
	}

	void Mesh::Color(DirectX::XMFLOAT4 color)
	{
		mPixelPerInstanceConstants.color = color;
	}

	void Mesh::Specular(float reflectiveness, float reflectionPower)
	{
		mPixelPerInstanceConstants.reflectiveness = reflectiveness;
		mPixelPerInstanceConstants.reflectionPower = reflectionPower;
	}

	std::unique_ptr<Dx::Drawables::Drawable> Mesh::Clone()
	{
		std::unique_ptr<Mesh> clone = std::make_unique<Mesh>(mName, mInputLayout, mVertexBuffer, mIndexBuffer, mVertexShader, mPixelShader);
		clone->mPixelPerInstanceConstants = this->mPixelPerInstanceConstants;
		clone->mVertexPerInstanceConstants = this->mVertexPerInstanceConstants;
		clone->mInitialized = this->mInitialized;

		return clone;
	}

	void Mesh::RegisterResources(){}

	void Mesh::UpdateConstants(DirectX::CXMMATRIX worldTransform)
	{
		mVertexPerInstanceConstants.worldTransform = worldTransform;

		std::static_pointer_cast<VSConstantBuffer<WorldTransform>>		(mVsConstantBuffer)->Update(mVertexPerInstanceConstants);
		std::static_pointer_cast<PSConstantBuffer<ColorSpecular>>		(mPsConstantBuffer)->Update(mPixelPerInstanceConstants);
	}
}