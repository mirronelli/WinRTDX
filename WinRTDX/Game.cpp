#include "pch.h"
#include "Game.h"

using namespace winrt::Windows::UI::Core;

Game::Game(CoreWindow const& window) :
	m_parentWindow(window)
{
}

void Game::Init()
{
	m_deviceResources = std::make_shared<Dx::Graphics>();
	m_deviceResources->SetWindow(m_parentWindow);
}

void Game::Run()
{
	while (!m_isClosing)
	{
		ProcessEvents();
		Update();
		Render();
		Present();
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
	m_deviceResources->StartFrame();
	DXGI_RGBA color{ 1,1,0.5,0.5 };
	m_deviceResources->SetColor(color);
}

void Game::Present()
{
	m_deviceResources->Present();
}

void Game::Close()
{
	m_isClosing = true;
}

void Game::Resize()
{
	m_deviceResources->Resize();
}
