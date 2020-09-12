#pragma once
#include "Graphics.h"
#include "winrt/Windows.UI.Core.h"

class Game
{
public:
	Game(winrt::Windows::UI::Core::CoreWindow const & window);
	void Init();
	void Run();
	void Close();
	void Resize();

private:
	void ProcessEvents();
	void Update();
	void Render();
	void Present();

	std::shared_ptr<Dx::Graphics> m_deviceResources;
	winrt::Windows::UI::Core::CoreWindow m_parentWindow{ nullptr };
	bool m_isClosing = false;
};

