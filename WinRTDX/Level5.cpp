#include "pch.h"
#include "Level5.h"
#include "IO.h"
#include "Cube.h"
#include "CubeTextured.h"
#include <random>

concurrency::task<void> Dx::Levels::Level5::Load()
{
	return concurrency::create_task([this]
		{
			mVertexShaderTextured = VertexShader::Preload(VertexType::TexturedWithNormal, L"VertexShader5_6.cso");
			mPixelShaderTextured = PixelShader::Preload(VertexType::TexturedWithNormal, L"PixelShader5_6.cso");

			mTexture = Texture::Preload("karin", L"Assets\\karin3.dds");
		}
	);
}

void Dx::Levels::Level5::SetupModel()
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 generator(rd());
	std::uniform_real_distribution<float> location(-240.0f, 240.0f);
	std::uniform_real_distribution<float> movementSpeed(.0f, .0f);
	std::uniform_real_distribution<float> startAngle(-DirectX::XM_PI, DirectX::XM_PI);
	std::uniform_real_distribution<float> rotationSpeed(-DirectX::XM_PI / 10.f, DirectX::XM_PI / 10.f);
	std::uniform_real_distribution<float> scale(3.f, 3.0f);

	for (int i = 0; i <= 5000; i++)
	{
		auto cube = std::make_unique<CubeTextured>(mVertexShaderTextured, mPixelShaderTextured);

		cube->X(location(generator));
		cube->Y(location(generator));
		cube->Z(location(generator));

		cube->SpeedX(movementSpeed(generator));
		cube->SpeedY(movementSpeed(generator));
		cube->SpeedZ(movementSpeed(generator));

		cube->RotationX(startAngle(generator));
		cube->RotationY(startAngle(generator));
		cube->RotationZ(startAngle(generator));

		cube->RotationSpeedX(rotationSpeed(generator));
		cube->RotationSpeedY(rotationSpeed(generator));
		cube->RotationSpeedZ(rotationSpeed(generator));

		cube->ScaleX(scale(generator));
		cube->ScaleY(scale(generator));
		cube->ScaleZ(scale(generator));

		cube->Texture(mTexture);
		cube->Init();

		mDrawables.push_back(std::move(cube)); 
	}

	for (auto d : mDrawables) {
		d->RegisterResources();
	}

	m_worldViewTransformConstantBuffer = VSConstantBuffer<DirectX::XMMATRIX>::Create(m_worldViewTransform, ResourceSlots::PerLevel, false);
	m_worldViewTransformConstantBuffer->Attach();

	m_worldRotationSpeedY = 0.1f;
}

void Dx::Levels::Level5::Update(float delta)
{
	m_worldRotationX = fmod(m_worldRotationX + delta * m_worldRotationSpeedX * DirectX::XM_2PI, DirectX::XM_2PI);
	m_worldRotationY = fmod(m_worldRotationY + delta * m_worldRotationSpeedY * DirectX::XM_2PI, DirectX::XM_2PI);
	m_worldRotationZ = fmod(m_worldRotationZ + delta * m_worldRotationSpeedZ * DirectX::XM_2PI, DirectX::XM_2PI);

	m_worldViewTransform = DirectX::XMMatrixIdentity();
	if (m_worldRotationZ != 0)
		m_worldViewTransform *= DirectX::XMMatrixRotationZ(m_worldRotationZ);

	if (m_worldRotationX != 0)
		m_worldViewTransform *= DirectX::XMMatrixRotationX(m_worldRotationX);

	if (m_worldRotationY != 0)
		m_worldViewTransform *= DirectX::XMMatrixRotationY(m_worldRotationY);

	m_worldViewTransform *= DirectX::XMMatrixPerspectiveFovLH(1.2f, Graphics::Instance->Width() / Graphics::Instance->Height(), .1f, 250.0f);
	m_worldViewTransformConstantBuffer->Update(m_worldViewTransform);


	for (auto d : mDrawables)
		d->Update(delta, XMMatrixIdentity());
}

void Dx::Levels::Level5::Render()
{
	float color[4]{ .2f, .3f, .1f, .2f };
	Graphics::Instance->StartFrame(color);

	for (auto d : mDrawables)
		d->Draw();
}