#include "pch.h"
#include "Level4.h"
#include "IO.h"
#include "Cube.h"
#include <random>

concurrency::task<void> Dx::Levels::Level4::Load()
{
	return concurrency::create_task([this]
		{
			m_VertexShader = VertexShader::Load(0, false, m_graphics, L"VertexShader4.cso");
			m_PixelShader = PixelShader::Load(1, false, m_graphics, L"PixelShader4.cso");
		}
	);
}

void Dx::Levels::Level4::SetupModel()
{
	//VertexShader::ClearCache();

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 generator(rd());
	std::uniform_real_distribution<float> location(-100.0, 100.0);
	std::uniform_real_distribution<float> movementSpeed(.0, .0);
	std::uniform_real_distribution<float> startAngle(.0, DirectX::XM_2PI);
	std::uniform_real_distribution<float> rotationSpeed(0.,2);

	for (int i = 0; i <= 5000; i++)
	{
		m_drawables.push_back(std::make_unique<Cube>(
			m_graphics, m_VertexShader, m_PixelShader,
			location(generator), location(generator)/2, location(generator)/2 + 50,
			movementSpeed(generator), movementSpeed(generator), movementSpeed(generator),
			startAngle(generator), startAngle(generator), startAngle(generator),
			rotationSpeed(generator), rotationSpeed(generator), rotationSpeed(generator)
			)
		);
	}

	for (auto d : m_drawables) {
		d->RegisterResources();
	}

	m_VertexShader->Attach(true);
	m_PixelShader->Attach(true);
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
		d->Draw();
}