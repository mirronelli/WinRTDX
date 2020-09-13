#include "pch.h"
#include "Level1.h"

concurrency::task<void> Dx::Levels::Level1::Load()
{
	return concurrency::create_task([this]
		{
			m_graphics->SetVertexShader(m_graphics->LoadVertexShader(L"VertexShader.cso"));
			m_graphics->SetPixelShader(m_graphics->LoadPixelShader(L"PixelShader.cso"));
		}
	);
}

void Dx::Levels::Level1::Render()
{
}

void Dx::Levels::Level1::Update(UINT32 deltaTime)
{
}
