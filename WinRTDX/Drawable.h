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

			if (mTexture != nullptr)
				mTexture->Attach();

			if (mVertexShader != nullptr)
				mVertexShader->Attach();

			if (mPixelShader != nullptr)
				mPixelShader->Attach();
		}

		void Draw() {
			assert(mInitialized);

			UpdateConstants(mTransform);
			AttachResources(false);
			Graphics::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			Graphics::Context->DrawIndexed(mIndicesCount, 0, 0);
		};

		void Texture(std::shared_ptr<Attachables::Texture> value) { mTexture = value; }

	protected:
		std::shared_ptr<PixelShader>					mPixelShader;
		std::shared_ptr<VertexShader>					mVertexShader;
		std::shared_ptr<Attachable>					mVertexBuffer;
		std::shared_ptr<IndexBuffer>					mIndexBuffer;
		std::shared_ptr<InputLayout>					mInputLayout;

		std::shared_ptr<Attachables::Texture>		mTexture;

		std::shared_ptr<Attachable>					mPsConstantBuffer;
		std::shared_ptr<Attachable>					mVsConstantBuffer;

		UINT mIndicesCount = 0;
		bool mInitialized = false;
	};
}