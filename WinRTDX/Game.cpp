#include "pch.h"
#include "Game.h"

using namespace winrt::Windows::UI::Core;

Game::Game(std::shared_ptr<Dx::DeviceResources> const& deviceResources, CoreWindow const& window) :
	m_deviceResources(deviceResources),
	m_parentWindow(window)
{
}

void Game::Run()
{
	while (!m_isClosing)
	{
		m_parentWindow.Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
		Update();
		Render();
		Present();
	}
}

void Game::Update()
{}

void Game::Render()
{}

void Game::Present()
{}

void Game::Close()
{
	m_isClosing = true;
}