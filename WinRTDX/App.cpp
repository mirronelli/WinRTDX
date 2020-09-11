#include "pch.h"
#include <DeviceResources.h>
#include <Game.h>

using namespace winrt;

using namespace Windows;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation::Numerics;
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
		m_deviceResources = std::make_shared<Dx::DeviceResources>();
	}

	void SetWindow(CoreWindow const& window)
	{
		m_window = window;
		m_window.Closed({ this, &App::OnWindowClosed });
		m_window.SizeChanged({ this, &App::OnWindowSizeChanged });

		m_deviceResources->SetWindow(window);
	}

	void Load(hstring const&)
	{
		if (!m_game)
		{
			m_game = std::make_unique<Game>(m_deviceResources, m_window);
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
		//m_deviceResources->SetLogicalSize(args.Size());
		//m_game->Resize();
		OutputDebugStringA((std::to_string(args.Size().Width) + "\n").c_str());
	}

	void OnDpiChanged(DisplayInformation const& sender, IInspectable const& /* args */)
	{
		m_deviceResources->SetDpi(sender.LogicalDpi());
		m_main->CreateWindowSizeDependentResources();
	}

#pragma endregion

private:
	std::shared_ptr<Dx::DeviceResources> m_deviceResources;
	std::unique_ptr<Game> m_game;
	CoreWindow m_window{ nullptr };
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	CoreApplication::Run(make<App>());
}
