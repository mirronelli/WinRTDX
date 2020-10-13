#include "pch.h"

#include "Game.h"
#include "LevelBase.h"
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
	mWindow(window)
{
}

void Game::Init()
{
	Dx::Graphics::CreateInstance();
	Dx::Graphics::Instance->SetWindow(mWindow);
	mKeyboardInput = std::make_shared<Dx::KeyboardInput>(mWindow);
	mMouseInput = std::make_shared<Dx::MouseInput>(mWindow);
}

void Game::LoadLevel(byte name)
{
	//ResourceManager::ClearCache();
	//name = 4;
	switch (name) {
	case 1:
		mLevel = std::make_unique<Dx::Levels::Level1>(mKeyboardInput, mMouseInput);
		break;
	case 2:
		mLevel = std::make_unique<Dx::Levels::Level2>(mKeyboardInput, mMouseInput);
		break;
	case 3:
		mLevel = std::make_unique<Dx::Levels::Level3>(mKeyboardInput, mMouseInput);
		break;
	case 4:
		mLevel = std::make_unique<Dx::Levels::Level4>(mKeyboardInput, mMouseInput);
		break;
	case 5:
		mLevel = std::make_unique<Dx::Levels::Level5>(mKeyboardInput, mMouseInput);
		break;
	case 6:
		mLevel = std::make_unique<Dx::Levels::Level6>(mKeyboardInput, mMouseInput);
		break;
	case 7:
		mLevel = std::make_unique<Dx::Levels::Level7>(mKeyboardInput, mMouseInput);
		break;
	case 8:
		mLevel = std::make_unique<Dx::Levels::Level8>(mKeyboardInput, mMouseInput);
		break;
	case 9:
		mLevel = std::make_unique<Dx::Levels::Level9>(mKeyboardInput, mMouseInput);
		break;
	default:
		mLevel = std::make_unique<Dx::Levels::Level9>(mKeyboardInput, mMouseInput);
		break;
	}

	concurrency::task<void> loading = mLevel->Load();
	while (!loading.is_done()) {
		ProcessEvents();
	}

	//mLevel->SetupModel();
}

void Game::Run()
{
	byte level = 0;

	while (!mIsClosing)
	{
		if (level != mCurrentLevel)
		{
			level = mCurrentLevel;
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
	if (mKeyboardInput->IsSet(Windows::System::VirtualKey::Space, true))
	{
		mCurrentLevel++;
		if (mCurrentLevel > mMaxLevel)
			mCurrentLevel = 1;
		mStop = false;
	}

	if (mKeyboardInput->IsSet(Windows::System::VirtualKey::P, true))
	{
		mStop = !mStop;
	}
}

void Game::ProcessEvents()
{
	mWindow.Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
}

void Game::Update(Dx::StepTimer const& timer)
{
	float delta = float(timer.GetElapsedSeconds());

	mLevel->Update(delta);

	//std::wostringstream debug;
	//debug << "Frame: " << mFrame << " Delta: " << delta << " FPS: " << mTimer.GetFramesPerSecond() << "\n";
	//OutputDebugString(debug.str().c_str());
}

void Game::Tick()
{
	mTimer.Tick([&]()
		{
			if (!mStop)
				Update(mTimer);
		}
	);
	mFrame++;
}

void Game::Render()
{
	mLevel->Render();
}

void Game::Present()
{
	Dx::Graphics::Instance->Present();
}

void Game::Close()
{
	mIsClosing = true;
}

void Game::Resize()
{
	Dx::Graphics::Instance->Resize();
}
