#pragma once
#include "Graphics.h"
#include "ILevel.h"
#include "StepTimer.h"

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
	void Update(Dx::StepTimer const& timer);
	void Tick();
	void Render();
	void Present();

	void KeyUp(CoreWindow window, KeyEventArgs args);

	std::shared_ptr<Dx::Graphics>						m_graphics;
	std::unique_ptr<Dx::Levels::ILevel>				m_level{ nullptr };
	CoreWindow												m_parentWindow{ nullptr };
	bool														m_isClosing = false;
	Dx::StepTimer											m_timer;
	ULONG64													m_frame = 0;

	byte														m_currentLevel = 4;
	byte														m_maxLevel = 4;
};

