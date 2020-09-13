#include "pch.h"
#include "Game.h"
#include "Loader.h"
#include <ppltasks.h>

using namespace winrt::Windows::UI::Core;

Game::Game(CoreWindow const& window) :
	m_parentWindow(window)
{
}

void Game::Init()
{
	m_graphics = std::make_shared<Dx::Graphics>();
	m_graphics->SetWindow(m_parentWindow);
}

concurrency::task<void> Game::SetupPipelineAsync()
{
	return concurrency::create_task([this]
		{
			m_graphics->SetVertexShader( m_graphics->LoadVertexShader(L"VertexShader.cso"));
			m_graphics->SetPixelShader(m_graphics->LoadPixelShader(L"PixelShader.cso"));
		}
	);
}

void Game::Run()
{
	concurrency::task setup = SetupPipelineAsync();

	while (!m_isClosing)
	{
		ProcessEvents();

		if (setup.is_done())
		{
			Update();
			Render();
			Present();
		}
	}
}

void Game::ProcessEvents()
{
	m_parentWindow.Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
}

void Game::Update()
{
}

void Game::Render()
{
	m_graphics->StartFrame();
	DXGI_RGBA color{ 1,1,0.5,0.5 };
	m_graphics->SetColor(color);
}

void Game::Present()
{
	m_graphics->Present();
}

void Game::Close()
{
	m_isClosing = true;
}

void Game::Resize()
{
	m_graphics->Resize();
}
