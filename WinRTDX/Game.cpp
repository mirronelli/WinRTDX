#include "pch.h"
#include "Game.h"
#include "Loader.h"
#include "ILevel.h"
#include "Level1.h"



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

void Game::LoadLevel(std::wstring name)
{
	m_level = std::make_unique<Dx::Levels::Level1>(m_graphics);
	concurrency::task<void> loading = m_level->Load();
	while (!loading.is_done())
		ProcessEvents();
}

void Game::Run()
{
	LoadLevel(L"1");

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
	m_graphics->StartFrame();
	DXGI_RGBA color{ 1, .5, .5, 0 };
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
