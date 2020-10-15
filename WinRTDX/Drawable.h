#pragma once
#include "pch.h"
#include "InputLayout.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "PSConstantBuffer.h"
#include "VSConstantBuffer.h"
#include "Texture.h"
#include "Sampler.h"
#include "ObjectInSpace.h"

using namespace Dx::Attachables;

#define MAKEID(A,B) std::string(A) + std::to_string(B)

namespace Dx::Drawables
{

	class Drawable : public ObjectInSpace
	{
	public:
		Drawable(
			std::shared_ptr<VertexShader> vertexShader,
			std::shared_ptr<PixelShader> pixelShader
		) :
			mVertexShader(vertexShader),
			mPixelShader(pixelShader)
		{};

		virtual ~Drawable() {};
		virtual void Init() { RegisterResources(); mInitialized = true; };
		virtual void RegisterResources() = 0;
		virtual void UpdateConstants(DirectX::CXMMATRIX) = 0;
		virtual std::unique_ptr<Drawable> Clone() { return nullptr; };

		void AttachResources(bool ){
			if (mVertexBuffer != nullptr)
				mVertexBuffer->Attach();

			if (mIndexBuffer != nullptr)
				mIndexBuffer->Attach();

			if (mInputLayout != nullptr)
				mInputLayout->Attach();

			if (mPsConstantBuffer != nullptr)
				mPsConstantBuffer->Attach();

			if (mVsConstantBuffer != nullptr)
				mVsConstantBuffer->Attach();

			if (mDiffuseTexture != nullptr)
				mDiffuseTexture->Attach(TextureType::Diffuse);

			if (mSpecularTexture != nullptr)
				mSpecularTexture->Attach(TextureType::Specular);

			if (mNormalTexture != nullptr)
				mNormalTexture->Attach(TextureType::Normal);

			if (mVertexShader != nullptr)
				mVertexShader->Attach();

			if (mPixelShader != nullptr)
				mPixelShader->Attach();

			if (mSampler != nullptr)
				mSampler->Attach();
		}

		void Draw() {
			assert(mInitialized);

			UpdateConstants(mTransform);
			AttachResources(false);
			Graphics::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			Graphics::Context->DrawIndexed(mIndexBuffer->Count(), 0, 0);
		};

		void DiffuseTexture(std::shared_ptr<Attachables::Texture> value) { mDiffuseTexture = value; }
		void SpecularTexture(std::shared_ptr<Attachables::Texture> value) { mSpecularTexture = value; }
		void NormalTexture(std::shared_ptr<Attachables::Texture> value) { mNormalTexture = value; }

	protected:
		std::shared_ptr<PixelShader>					mPixelShader;
		std::shared_ptr<VertexShader>					mVertexShader;
		std::shared_ptr<VertexBufferBase>			mVertexBuffer;
		std::shared_ptr<IndexBuffer>					mIndexBuffer;
		std::shared_ptr<InputLayout>					mInputLayout;

		std::shared_ptr<Texture>						mDiffuseTexture;
		std::shared_ptr<Texture>						mSpecularTexture;
		std::shared_ptr<Texture>						mNormalTexture;
		std::shared_ptr<Sampler>						mSampler;

		std::shared_ptr<Attachable>					mPsConstantBuffer;
		std::shared_ptr<Attachable>					mVsConstantBuffer;

		bool mInitialized = false;
	};
}