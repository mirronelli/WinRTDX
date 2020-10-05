#include "pch.h"

#include "Game.h"
#include "ILevel.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include "Level6.h"
#include "Level7.h"
#include "Level8.h"
#include "Level9.h"

Game::Game(CoreWindow const& window) :
	m_window(window)
{
}

void Game::Init()
{
	Dx::Graphics::CreateInstance();
	Dx::Graphics::Instance->SetWindow(m_window);
	m_keyboardInput = std::make_shared<Dx::KeyboardInput>(m_window);
	m_mouseInput = std::make_shared<Dx::MouseInput>(m_window);
}

void Game::LoadLevel(byte name)
{
	//ResourceManager::ClearCache();
	//name = 4;
	switch (name) {
	case 1:
		m_level = std::make_unique<Dx::Levels::Level1>(m_keyboardInput, m_mouseInput);
		break;
	case 2:
		m_level = std::make_unique<Dx::Levels::Level2>(m_keyboardInput, m_mouseInput);
		break;
	case 3:
		m_level = std::make_unique<Dx::Levels::Level3>(m_keyboardInput, m_mouseInput);
		break;
	case 4:
		m_level = std::make_unique<Dx::Levels::Level4>(m_keyboardInput, m_mouseInput);
		break;
	case 5:
		m_level = std::make_unique<Dx::Levels::Level5>(m_keyboardInput, m_mouseInput);
		break;
	case 6:
		m_level = std::make_unique<Dx::Levels::Level6>(m_keyboardInput, m_mouseInput);
		break;
	case 7:
		m_level = std::make_unique<Dx::Levels::Level7>(m_keyboardInput, m_mouseInput);
		break;
	case 8:
		m_level = std::make_unique<Dx::Levels::Level8>(m_keyboardInput, m_mouseInput);
		break;
	case 9:
		m_level = std::make_unique<Dx::Levels::Level9>(m_keyboardInput, m_mouseInput);
		break;
	default:
		m_level = std::make_unique<Dx::Levels::Level9>(m_keyboardInput, m_mouseInput);
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
		ProcessKeyboard();
		Tick();
		Render();
		Present();
	}
}

void Game::ProcessKeyboard()
{
	if (m_keyboardInput->IsSet(Windows::System::VirtualKey::Space, true))
	{
		m_currentLevel++;
		if (m_currentLevel > m_maxLevel)
			m_currentLevel = 1;
		m_stop = false;
	}

	if (m_keyboardInput->IsSet(Windows::System::VirtualKey::P, true))
	{
		m_stop = !m_stop;
	}
}

void Game::ProcessEvents()
{
	m_window.Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
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
			if (!m_stop)
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
	Dx::Graphics::Instance->Present();
}

void Game::Close()
{
	m_isClosing = true;
}

void Game::Resize()
{
	Dx::Graphics::Instance->Resize();
}
