#include "pch.h"
#include "Game.h"
#include "ILevel.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

using namespace winrt::Windows::UI::Core;

Game::Game(CoreWindow const& window) :
	m_parentWindow(window)
{
}

void Game::Init()
{
	m_graphics = std::make_shared<Dx::Graphics>();
	m_graphics->SetWindow(m_parentWindow);
	m_parentWindow.KeyUp({ this, &Game::KeyUp });
}

void Game::LoadLevel(byte name)
{
	switch (name) {
	case 1:
		m_level = std::make_unique<Dx::Levels::Level1>(m_graphics);
		break;
	case 2:
		m_level = std::make_unique<Dx::Levels::Level2>(m_graphics);
		break;
	case 3:
		m_level = std::make_unique<Dx::Levels::Level3>(m_graphics);
		break;
	}

	concurrency::task<void> loading = m_level->Load();
	while (!loading.is_done()) {
		ProcessEvents();
	}

	m_level->SetupModel();
}

void Game::Run()
{
	byte level = 0;

	while (!m_isClosing)
	{
		if (level != m_currentLevel)
		{
			level = m_currentLevel;
			LoadLevel(level);
		}

		ProcessEvents();
		Tick();
		Render();
		Present();
	}
}

void Game::ProcessEvents()
{
	m_parentWindow.Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
}

void Game::Update(Dx::StepTimer const& timer)
{
	float delta = float(timer.GetElapsedSeconds());

	m_level->Update(delta);

	std::wostringstream debug;
	debug << "Frame: " << m_frame << " Delta: " << delta << " FPS: " << m_timer.GetFramesPerSecond() << "\n";
	OutputDebugString(debug.str().c_str());
}

void Game::Tick()
{
	m_timer.Tick([&]()
		{
			Update(m_timer);
		}
	);
	m_frame++;
}

void Game::Render()
{
	m_level->Render();
}

void Game::Present()
{
	m_graphics->Present();
}

void Game::KeyUp(CoreWindow window, KeyEventArgs args)
{
	if (args.VirtualKey() == winrt::Windows::System::VirtualKey::Space)
	{
		m_currentLevel++;
		if (m_currentLevel > m_maxLevel)
			m_currentLevel = 1;
	}
}

void Game::Close()
{
	m_isClosing = true;
}

void Game::Resize()
{
	m_graphics->Resize();
}
