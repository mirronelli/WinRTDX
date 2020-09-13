#pragma once
#include "Graphics.h"
#include "ILevel.h"

class Game
{
public:
	Game(winrt::Windows::UI::Core::CoreWindow const & window);
	void Init();
	void LoadLevel(std::wstring name);
	void Run();
	void Close();
	void Resize();

private:
	void ProcessEvents();
	void Update();
	void Render();
	void Present();

	std::shared_ptr<Dx::Graphics>						m_graphics;
	std::unique_ptr<Dx::Levels::ILevel>				m_level{ nullptr };
	CoreWindow												m_parentWindow{ nullptr };
	bool														m_isClosing = false;
};

