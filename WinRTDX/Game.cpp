#include "pch.h"
#include "Game.h"
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
	m_parentWindow.KeyUp({ this, &Game::KeyUp });
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.f / 60.f);
}

void Game::LoadLevel(std::wstring name)
{
	m_level = std::make_unique<Dx::Levels::Level1>(m_graphics);
	concurrency::task<void> loading = m_level->Load();
	while (!loading.is_done()) {
		ProcessEvents();
	}

	m_level->SetupModel();
}

void Game::Run()
{
	LoadLevel(L"1");

	while (!m_isClosing)
	{
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
	debug << "Frame: " << m_frame << " Delta: " << delta << "\n";
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
	m_graphics->StartFrame();
	m_level->Render();
}

void Game::Present()
{
	m_graphics->Present();
}

void Game::KeyUp(CoreWindow window, KeyEventArgs args)
{
	if (args.VirtualKey() == winrt::Windows::System::VirtualKey::Space) {

		Windows::UI::Popups::MessageDialog box{ L"ehmm ok!" };
		box.ShowAsync();
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
