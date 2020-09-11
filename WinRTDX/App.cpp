#include "pch.h"
#include <DeviceResources.h>
#include <Game.h>

using namespace winrt;

using namespace Windows;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation; 
using namespace Windows::Foundation::Numerics;
using namespace Windows::Graphics::Display;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Composition;

struct App : implements<App, IFrameworkViewSource, IFrameworkView>
{
	IFrameworkView CreateView()
	{
		return *this;
	}

	void Initialize(CoreApplicationView const& applicationView)
	{
		applicationView.Activated({ this, &App::OnActivated });
	}

	void SetWindow(CoreWindow const& window)
	{
		m_window = window;
		m_window.Closed({ this, &App::OnWindowClosed });
		m_window.SizeChanged({ this, &App::OnWindowSizeChanged });
	}

	void Load(hstring const&)
	{
		if (!m_game)
		{
			m_game = std::make_unique<Game>(m_window);
			m_game->Init();
		}
	}

	void Uninitialize()
	{
		m_game->Close();
	}

	void Run()
	{
		m_game->Run();
	}

#pragma region Events

	void OnActivated(CoreApplicationView const& /* applicationView */, IActivatedEventArgs const& /* args */)
	{
		CoreWindow window = CoreWindow::GetForCurrentThread();
		window.Activate();
	}

	void OnWindowClosed(CoreWindow const& /* sender */, CoreWindowEventArgs const& /* args */)
	{
		m_game->Close();
	}
	
	void OnWindowSizeChanged(CoreWindow const& /* window */, WindowSizeChangedEventArgs const& args)
	{
		m_game->Resize();
	}

	void OnDpiChanged(DisplayInformation const& sender, IInspectable const& /* args */)
	{
		m_game->Resize();
	}

#pragma endregion

private:
	std::unique_ptr<Game> m_game;
	CoreWindow m_window{ nullptr };
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	CoreApplication::Run(make<App>());
}
