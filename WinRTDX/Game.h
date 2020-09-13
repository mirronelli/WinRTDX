#pragma once
#include "Graphics.h"
#include "winrt/Windows.UI.Core.h"
#include <ppltasks.h>

class Game
{
public:
	Game(winrt::Windows::UI::Core::CoreWindow const & window);
	void Init();
	concurrency::task<void> SetupPipelineAsync();
	void Run();
	void Close();
	void Resize();

private:
	void ProcessEvents();
	void Update();
	void Render();
	void Present();

	std::shared_ptr<Dx::Graphics> m_graphics;
	winrt::Windows::UI::Core::CoreWindow m_parentWindow{ nullptr };
	bool m_isClosing = false;
};

