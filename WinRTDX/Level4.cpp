#include "pch.h"
#include "Level4.h"
#include "IO.h"
#include "Cube.h"
#include <DirectXMath.h>
#include <random>

concurrency::task<void> Dx::Levels::Level4::Load()
{
	return concurrency::create_task([this]
		{
			m_compiledVertexShader = IO::ReadFile(L"VertexShader4.cso");
			m_graphics->SetVertexShader(m_graphics->CreateVertexShader(m_compiledVertexShader));

			m_compiledPixelShader = IO::ReadFile(L"PixelShader4.cso");
			m_graphics->SetPixelShader(m_graphics->CreatePixelShader(m_compiledPixelShader));
		}
	);
}


void Dx::Levels::Level4::SetupModel()
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 generator(rd());
	std::uniform_real_distribution<float> rotationSpeed(4., 8.);
	std::uniform_real_distribution<float> movementSpeed(.0, .0);
	std::uniform_real_distribution<float> location(-25.0, 25.0);
	std::uniform_real_distribution<float> startAngle(.0, DirectX::XM_2PI);

	for (int i = 0; i <= 128; i++)
	{
		m_drawables.push_back(std::make_unique<Cube>(
			m_graphics, m_compiledVertexShader, m_compiledPixelShader,
			location(generator), location(generator), location(generator) + 10,
			movementSpeed(generator), movementSpeed(generator), movementSpeed(generator),
			rotationSpeed(generator), rotationSpeed(generator), rotationSpeed(generator),
			startAngle(generator), startAngle(generator), startAngle(generator)
			)
		);

	for (auto d : m_drawables)
		d->RegisterResources();
	}
}

void Dx::Levels::Level4::Update(float delta)
{
	for (auto d : m_drawables)
		d->Update(delta);
}

void Dx::Levels::Level4::Render()
{
	float color[4]{ .2f, .2f, .3f, .2f};
	m_graphics->StartFrame(color);

	for (auto d : m_drawables)
	{
		d->AttachResources();
		d->Draw();
	}
}