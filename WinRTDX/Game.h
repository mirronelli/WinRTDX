#pragma once
#include "Graphics.h"
#include "ILevel.h"
#include "StepTimer.h"
#include "KeyMap.h"

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
	void Update(Dx::StepTimer const& timer, Dx::KeyMap keyMap);
	void Tick();
	void Render();
	void Present();

	void KeyUp(CoreWindow window, KeyEventArgs args);
	void KeyDown(CoreWindow window, KeyEventArgs args);

	std::shared_ptr<Dx::Graphics>						m_graphics;
	std::unique_ptr<Dx::Levels::ILevel>				m_level{ nullptr };
	CoreWindow												m_parentWindow{ nullptr };
	bool														m_isClosing = false;
	Dx::StepTimer											m_timer;
	ULONG64													m_frame = 0;

	byte														m_currentLevel = 6;
	byte														m_maxLevel = 6;
	bool														m_stop = false;

	Dx::KeyMap												m_keyMap;
};

