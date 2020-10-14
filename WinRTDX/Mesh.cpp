#include "pch.h"
#include "Mesh.h"


namespace Dx::Drawables
{
	Mesh::Mesh(
		std::string name, 
		std::shared_ptr<InputLayout> inputLayout, 
		std::shared_ptr<Dx::Attachables::VertexBufferBase> vertexBuffer, 
		std::shared_ptr<Dx::Attachables::IndexBuffer> indexBuffer, 
		std::shared_ptr<Dx::Attachables::VertexShader> vertexShader, 
		std::shared_ptr<Dx::Attachables::PixelShader> pixelShader,
		std::shared_ptr<Dx::Attachables::Texture> texture
	)
	: 
		mName(name), Drawable(vertexShader, pixelShader)
	{
		mVertexBuffer = vertexBuffer;
		mIndexBuffer = indexBuffer;
		mInputLayout = inputLayout;
		mTexture = texture;
		mPixelPerInstanceConstants.specularColor = DirectX::XMFLOAT4 { 1.,1.,1.,1.};

		mVsConstantBuffer = VSConstantBuffer<WorldTransform>		::Create(mVertexPerInstanceConstants, ResourceSlots::PerInstance);
		mPsConstantBuffer = PSConstantBuffer<AllColors>				::Create(mPixelPerInstanceConstants, ResourceSlots::PerInstance);
	}

	void Mesh::Color(DirectX::XMFLOAT4 color)
	{
		mPixelPerInstanceConstants.diffuseColor = color;
	}

	void Mesh::Specular(DirectX::XMFLOAT4 specularColor, float reflectiveness, float reflectionPower)
	{
		mPixelPerInstanceConstants.reflectiveness = reflectiveness;
		mPixelPerInstanceConstants.reflectionPower = reflectionPower;
	}

	std::unique_ptr<Dx::Drawables::Drawable> Mesh::Clone()
	{
		std::unique_ptr<Mesh> clone = std::make_unique<Mesh>(mName, mInputLayout, mVertexBuffer, mIndexBuffer, mVertexShader, mPixelShader, mTexture);
		clone->mPixelPerInstanceConstants = this->mPixelPerInstanceConstants;
		clone->mVertexPerInstanceConstants = this->mVertexPerInstanceConstants;
		clone->mInitialized = this->mInitialized;

		return clone;
	}

	void Mesh::RegisterResources(){}

	void Mesh::UpdateConstants(DirectX::CXMMATRIX worldTransform)
	{
		mVertexPerInstanceConstants.worldTransform = worldTransform;

		std::static_pointer_cast<VSConstantBuffer<WorldTransform>>	(mVsConstantBuffer)->Update(mVertexPerInstanceConstants);
		std::static_pointer_cast<PSConstantBuffer<AllColors>>			(mPsConstantBuffer)->Update(mPixelPerInstanceConstants);
	}
}