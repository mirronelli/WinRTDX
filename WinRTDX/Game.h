#pragma once
#include "Graphics.h"
#include "ILevel.h"
#include "StepTimer.h"
#include "KeyboardInput.h"

class Game
{
public:
	Game(winrt::Windows::UI::Core::CoreWindow const & window);
	void Init();
	void LoadLevel(byte name);
	void Run();
	void Close();
	void Resize();

private:
	void ProcessEvents();
	void ProcessKeyboard();
	void Update(Dx::StepTimer const& timer);
	void Tick();
	void Render();
	void Present();

	std::shared_ptr<Dx::Graphics>						m_graphics;
	std::unique_ptr<Dx::Levels::ILevel>				m_level{ nullptr };
	CoreWindow												m_window{ nullptr };
	bool														m_isClosing = false;
	Dx::StepTimer											m_timer;
	ULONG64													m_frame = 0;

	byte														m_currentLevel = 7;
	byte														m_maxLevel = 7;
	bool														m_stop = false;

	std::shared_ptr<Dx::KeyboardInput>				m_keyboardInput{ nullptr };
	std::shared_ptr<Dx::MouseInput>					m_mouseInput{ nullptr };
};

