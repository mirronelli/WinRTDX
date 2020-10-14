#include "pch.h"
#include "Level3.h"
#include "IO.h"
#include <DirectXMath.h>

using namespace Dx::Attachables;
using namespace Dx::Drawables;

concurrency::task<void> Dx::Levels::Level3::Load()
{
	return concurrency::create_task([this]
		{
			VertexShader::Preload(VertexType::Simple, L"VertexShader3.cso");
			PixelShader::Preload(VertexType::Simple, L"PixelShader3.cso");
			mVertexShader = VertexShader::Get(VertexType::Simple);
			mPixelShader = PixelShader::Get(VertexType::Simple);			

			SetupModel();
		}
	);
}

void Dx::Levels::Level3::CreateVertices()
{
	D3D11_INPUT_ELEMENT_DESC ieds[] =
	{
		{ "POSITION",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 0,										D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

	// define vertices and its buffer
	VertexSimple vertices[] = {
		{ -1.0f,	 1.0f,	-1.0f,	 1.0f },
		{ 1.0f,	 1.0f,	-1.0f,	 1.0f },
		{ 1.0f,	-1.0f,	-1.0f,	 1.0f },
		{ -1.0f,	-1.0f,	-1.0f,	 1.0f },
		{ -1.0f,	 1.0f,	 1.0f,	 1.0f },
		{ 1.0f,	 1.0f,	 1.0f,	 1.0f },
		{ 1.0f,	-1.0f,	 1.0f,	 1.0f },
		{ -1.0f,	-1.0f,	 1.0f,	 1.0f },
	};

	D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
	vertexBufferDesc.ByteWidth = sizeof(VertexSimple) * ARRAYSIZE(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srdVertices = { vertices, 0, 0 };

	Graphics::Device->CreateBuffer(&vertexBufferDesc, &srdVertices, mVertexBuffer.put());
}

void Dx::Levels::Level3::CreateIndices()
{
	const unsigned short indices[]{
		0, 1, 2,  // front
		0, 2, 3,
		1, 5, 2,  // right
		5, 6, 2,
		0, 7, 4,  // left
		0, 3, 7,
		0,	5, 1,  // top
		0, 4, 5,
		3, 2, 7,  // bottom
		2, 6, 7,
		4, 6, 5,  // back
		4, 7, 6
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

void Dx::Levels::Level3::CreateConstantData()
{
	D3D11_BUFFER_DESC	transformDesc{ 0 };
	transformDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	transformDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	transformDesc.CPUAccessFlags = 0;
	transformDesc.ByteWidth = sizeof(mConstantData);

	D3D11_SUBRESOURCE_DATA transformSrd{ 0 };
	transformSrd.pSysMem = &mConstantData;

	Graphics::Device->CreateBuffer(&transformDesc, &transformSrd, mConstantBuffer.put());
}

void Dx::Levels::Level3::RegisterBuffers()
{
	UINT strideVertices = sizeof(VertexSimple);
	UINT offsetVertices = 0;
	ID3D11Buffer* vertexBuffers[1] = { mVertexBuffer.get() };
	Graphics::Context->IASetVertexBuffers(0, 1, vertexBuffers, &strideVertices, &offsetVertices);

	Graphics::Context->IASetIndexBuffer(mIndexBuffer.get(), DXGI_FORMAT::DXGI_FORMAT_R16_UINT, 0);

	ID3D11Buffer* constantBuffers[1] = { mConstantBuffer.get() };
	Graphics::Context->VSSetConstantBuffers(0, 1, constantBuffers);
	Graphics::Context->PSSetConstantBuffers(0, 1, constantBuffers);
}

void Dx::Levels::Level3::SetupModel()
{
	Dx::Attachables::Attachable::Reset();
	CreateVertices();
	CreateIndices();
	CreateConstantData();
	RegisterBuffers();
}

void Dx::Levels::Level3::Update(float delta)
{
	mElapsedTime += delta;
	while (mElapsedTime > mEffectDuration)
		mElapsedTime -= mEffectDuration;

	mProgress = mElapsedTime / mEffectDuration * DirectX::XM_2PI;
}

void Dx::Levels::Level3::DrawCube(float angle, float x, float y, float z)
{
	Graphics::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mConstantData.matrix =
		DirectX::XMMatrixRotationX(angle) *
		DirectX::XMMatrixRotationZ(angle) *
		DirectX::XMMatrixRotationY(angle) *
		DirectX::XMMatrixTranslation(x, y, z) *
		DirectX::XMMatrixPerspectiveFovLH(1.2f, Graphics::Instance->Width() / Graphics::Instance->Height(), 0.1f, 100.0f);
	Graphics::Context->UpdateSubresource(mConstantBuffer.get(), 0, 0, &mConstantData, 0, 0);
	Graphics::Context->DrawIndexed(36, 0, 0);
}

void Dx::Levels::Level3::Render()
{
	float color[4]{ .2f, .2f, .2f, .2f};
	Graphics::Instance->StartFrame(color);

	DrawCube(mProgress, Graphics::Instance->MouseX() -.5f, Graphics::Instance->MouseY(), 8.0f);
	DrawCube(-mProgress, Graphics::Instance->MouseX() +.5f, 0, Graphics::Instance->MouseY() + 8.0f);
}