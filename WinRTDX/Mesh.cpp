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
		std::shared_ptr<Dx::Attachables::Texture> diffuseTexture,
		std::shared_ptr<Dx::Attachables::Texture> specularTexture,
		std::shared_ptr<Dx::Attachables::Texture> normalTexture,
		std::shared_ptr<Dx::Attachables::Sampler> sampler
	)
	: 
		mName(name), Drawable(vertexShader, pixelShader)
	{
		mVertexBuffer = vertexBuffer;
		mIndexBuffer = indexBuffer;
		mInputLayout = inputLayout;
		mDiffuseTexture = diffuseTexture;
		mSpecularTexture = specularTexture;
		mNormalTexture = normalTexture;
		mSampler = sampler;

		mVsConstantBuffer = VSConstantBuffer<WorldTransform>		::Create(mVertexPerInstanceConstants, ResourceSlots::PerInstance);
		mPsConstantBuffer = PSConstantBuffer<PixelShaderInstanceConstants>				::Create(mPixelPerInstanceConstants, ResourceSlots::PerInstance);

		mPixelPerInstanceConstants.specularColor = DirectX::XMFLOAT4 { 1.,1.,1.,1.};
	}

	void Mesh::InstanceConstants(PixelShaderInstanceConstants value)
	{
		mPixelPerInstanceConstants = value;
	}

	void Mesh::Color(DirectX::XMFLOAT4 color)
	{
		mPixelPerInstanceConstants.diffuseColor = color;
	}

	void Mesh::Specular(DirectX::XMFLOAT4 specularColor, float specularPower)
	{
		mPixelPerInstanceConstants.specularColor = specularColor;
		mPixelPerInstanceConstants.specularPower = specularPower;
	}

	std::unique_ptr<Dx::Drawables::Drawable> Mesh::Clone()
	{
		std::unique_ptr<Mesh> clone = std::make_unique<Mesh>(
			mName, 
			mInputLayout, 
			mVertexBuffer, 
			mIndexBuffer, 
			mVertexShader, 
			mPixelShader, 
			mDiffuseTexture, 
			mSpecularTexture, 
			mNormalTexture, 
			mSampler
		);

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
		std::static_pointer_cast<PSConstantBuffer<PixelShaderInstanceConstants>>			(mPsConstantBuffer)->Update(mPixelPerInstanceConstants);
	}
}