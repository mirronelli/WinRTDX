#include "pch.h"
#include "Level4.h"
#include "IO.h"
#include "Cube.h"
#include <random>

concurrency::task<void> Dx::Levels::Level4::Load()
{
	return concurrency::create_task([this]
		{
			Dx::Attachables::ResourceManager::ClearCache();
			m_vertexShaderSimple = VertexShader::Load(1, false, m_graphics, L"VertexShader4.cso");
			m_pixelShaderSimple = PixelShader::Load(1, false, m_graphics, L"PixelShader4.cso");
		}
	);
}

void Dx::Levels::Level4::SetupModel()
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 generator(rd());
	std::uniform_real_distribution<float> location(-140.0f, 140.0f);
	std::uniform_real_distribution<float> movementSpeed(.0f, .0f);
	std::uniform_real_distribution<float> startAngle(-DirectX::XM_PI, DirectX::XM_PI);
	std::uniform_real_distribution<float> rotationSpeed(-DirectX::XM_PI/10.f, DirectX::XM_PI/10.f);
	std::uniform_real_distribution<float> scale(0.2f,4.0f);

	for (int i = 0; i <= 5000; i++)
	{
		m_drawables.push_back(std::make_unique<Cube>(
			m_graphics, m_vertexShaderSimple, m_pixelShaderSimple, nullptr,
			location(generator), location(generator), location(generator),
			movementSpeed(generator), movementSpeed(generator), movementSpeed(generator),
			startAngle(generator), startAngle(generator), startAngle(generator),
			rotationSpeed(generator), rotationSpeed(generator), rotationSpeed(generator),
			scale(generator), scale(generator), scale(generator)
			)
		);
	}

	for (auto d : m_drawables) {
		d->RegisterResources();
	}

	m_worldViewTransformConstantBuffer = VSConstantBuffer<DirectX::XMMATRIX>::Create(1, false, m_graphics, m_worldViewTransform, 0, false);
	m_worldViewTransformConstantBuffer->Attach(false);

	m_worldRotationSpeedY = 0.01f;
}

void Dx::Levels::Level4::Update(float delta, KeyMap keyMap)
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

	m_worldViewTransform *= DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, m_graphics->Width() / m_graphics->Height(), 1.f, 100.0f);
	m_worldViewTransformConstantBuffer->Update(m_worldViewTransform);


	for (auto d : m_drawables)
		d->Update(delta);
}

void Dx::Levels::Level4::Render()
{
	float color[4]{ .2f, .2f, .3f, .2f};
	m_graphics->StartFrame(color);

	for (auto d : m_drawables)
		d->Draw();
}