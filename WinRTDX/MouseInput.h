#pragma once
#include "pch.h"

namespace Dx
{
	class MouseInput
	{
	public:
		MouseInput(CoreWindow window) : mWindow(window), mMouse(Windows::Devices::Input::MouseDevice::GetForCurrentView())
		{
			mWindow.PointerPressed({ this, &MouseInput::OnPointerPressed });
			mWindow.PointerReleased({ this, &MouseInput::OnPointerPressed });
			mWindow.PointerMoved({ this, &MouseInput::OnStandardMouseMoved });
			mMouse.MouseMoved({ this, &MouseInput::OnRelativeMouseMoved });
		}

		void RelativeTrackingEnter()
		{
			if (!mInRelativeTracking)
			{
				mInRelativeTracking = true;
				mOriginalCursor = mWindow.PointerCursor();
				mWindow.PointerCursor(nullptr);
			}
		}

		void RelativeTrackingExit()
		{
			if (mInRelativeTracking)
			{
				mInRelativeTracking = false;
				mWindow.PointerCursor(mOriginalCursor);
			}
		}

		float RelativeDeltaX()
		{
			float result = mInRelativeTracking ? mDeltaX : 0;
			mDeltaX = 0;
			return result;
		}

		float RelativeDeltaY()
		{
			float result = mInRelativeTracking ? mDeltaY : 0;
			mDeltaY = 0;
			return result;
		}

	private:
		void OnPointerPressed(CoreWindow window, PointerEventArgs args)
		{

		}

		void OnPointerReleased(CoreWindow window, PointerEventArgs args)
		{

		}

		void OnRelativeMouseMoved(Windows::Devices::Input::MouseDevice mouse, Windows::Devices::Input::MouseEventArgs args)
		{
			if (mInRelativeTracking)
			{
				mDeltaX += args.MouseDelta().X;
				mDeltaY += args.MouseDelta().Y;
			}
		}

		void OnStandardMouseMoved(CoreWindow window, PointerEventArgs args)
		{
			if (!mInRelativeTracking)
			{
				mPointerX = args.CurrentPoint().Position().X;
				mPointerY = args.CurrentPoint().Position().Y;
			}
		}
		CoreCursor										mOriginalCursor{ nullptr };
		CoreWindow										mWindow;
		Windows::Devices::Input::MouseDevice	mMouse;
		bool												mInRelativeTracking = false;
		float												mPointerX = 0;
		float												mPointerY = 0;
		float												mDeltaX = 0;
		float												mDeltaY = 0;
	};
}