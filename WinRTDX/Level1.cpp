#include "pch.h"
#include "Level1.h"
#include "IO.h"
#include "Structures.h"

using namespace Dx::Attachables;
using namespace Dx::Drawables;

concurrency::task<void> Dx::Levels::Level1::Load()
{
	return concurrency::create_task([this]
		{
			VertexShader::Preload (VertexType::Colored, L"VertexShader.cso");
			PixelShader::Preload(VertexType::Colored, L"PixelShader.cso");
			mVertexShader = VertexShader::Get(VertexType::Colored);
			mPixelShader = PixelShader::Get(VertexType::Colored);

			SetupModel();
		}
	);
}

void Dx::Levels::Level1::SetupModel()
{
	Dx::Attachables::Attachable::Reset();

	VertexSimple vertices[] = {
		{  -1.0f,  1.0f, 0.0f, 1.0f,		1.0f,  .0f,  .0f, 1.0f },
		{  -0.6f,  0.5f, 0.0f, 1.0f,		 .0f, 1.0f,  .0f, 1.0f },
		{  -0.8f,  0.2f, 0.0f, 1.0f,		 .0f,  .0f, 1.0f, 1.0f },
		{  -0.4f,  0.0f, 0.0f, 1.0f,		1.0f,  .0f,  .0f, 1.0f },
		{  -0.6f, -0.5f, 0.0f, 1.0f,		1.0f, 1.0f,  .0f, 1.0f },
		{   .45f, -0.8f, 0.0f, 1.0f,		0.0f,  .0f, 1.0f, 1.0f },
	};

	// Describe Input Layout for vertex shader
	D3D11_INPUT_ELEMENT_DESC ieds[] =
	{
		 {"POSITION",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,										D3D11_INPUT_PER_VERTEX_DATA, 0},
		 {"COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	com_ptr<ID3D11InputLayout> inputLayout;
	Graphics::Device->CreateInputLayout(
		ieds,
		ARRAYSIZE(ieds),
		mVertexShader->Data(),
		mVertexShader->Length(),
		inputLayout.put()
	);
	Graphics::Context->IASetInputLayout(inputLayout.get());

	// Create Vertex Buffer over vertices
	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(VertexSimple) * ARRAYSIZE(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA srdVertices = { vertices, 0, 0 };
	Graphics::Device->CreateBuffer(&vertexBufferDesc, &srdVertices, mVertexBuffer.put());
	
	// define indices
	const unsigned short indices[] = {
		0, 1, 2,
		3, 5, 4,
		5, 2, 1
	};

	D3D11_BUFFER_DESC indexBufferDesc = { 0 };
	indexBufferDesc.ByteWidth = sizeof(unsigned short) * ARRAYSIZE(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA srdIndices = { indices, 0, 0 };
	Graphics::Device->CreateBuffer(&indexBufferDesc, &srdIndices, mIndexBuffer.put());
	mVertexShader->Attach();
	mPixelShader->Attach();
}

void Dx::Levels::Level1::Render()
{
	float color[4]{ mRed * c_maxColorIntensity, mGreen * c_maxColorIntensity, mBlue * c_maxColorIntensity, .0 };
	Graphics::Instance->StartFrame(color);

	UINT strideVertices = sizeof(VertexSimple);
	UINT offsetVertices = 0;
	ID3D11Buffer* vertexBuffers[1] = { mVertexBuffer.get() };
	Graphics::Context->IASetVertexBuffers(0, 1, vertexBuffers, &strideVertices, &offsetVertices);
	Graphics::Context->IASetIndexBuffer(mIndexBuffer.get(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);
	Graphics::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Graphics::Context->DrawIndexed(9, 0, 0);
}

void Dx::Levels::Level1::Update(float delta)
{
	mElapsedTime += delta;
	if (mElapsedTime > mEffectDuration)
		mElapsedTime -= mEffectDuration;

	float progress = mElapsedTime / mEffectDuration;

	if (progress < c_oneSixth)														//phase 1	
	{
		mRed = progress * 6;															// red up	
		mGreen = 0;																		// green zero
		mBlue = 1.0;																		// blue 1
	}
	else if (progress < c_twoSixths)												//phase 2
	{
		mRed = 1.0f;																		// red 1
		mGreen = 0;																		// green zero
		mBlue = 1.0f - (progress-c_oneSixth) * 6;								// blue down
	}
	else if (progress < c_threeSixths)											//phase 3
	{
		mRed = 1.0;																		// red 1	
		mGreen = (progress - c_twoSixths) * 6;									// green up
		mBlue = 0;																			// blue 0
	}
	else if (progress < c_fourSixths)											// phase 4
	{
		mRed = 1.0f - (progress - c_threeSixths) * 6;							// red down
		mGreen = 1.0;																		// green 1
		mBlue = 0;																			// blue 0
	}
	else if (progress < c_fiveSixths)											// phase 5
	{
		mRed = 0;																			// red 0
		mGreen = 1;																		// green 1
		mBlue = (progress - c_fourSixths) * 6;									// blue up
	}
	else																					// phase 6
	{
		mRed = 0;																			// red 0
		mGreen = 1.0f -(progress - c_fiveSixths) * 6;							// green down
		mBlue = 1;																			// blue up
	}
}
