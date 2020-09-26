#pragma once
#include "pch.h"

namespace Dx
{
	class MouseInput
	{
	public:
		MouseInput(CoreWindow window) : m_window(window), m_mouse(Windows::Devices::Input::MouseDevice::GetForCurrentView())
		{
			m_window.PointerPressed({ this, &MouseInput::OnPointerPressed });
			m_window.PointerReleased({ this, &MouseInput::OnPointerPressed });
			m_window.PointerMoved({ this, &MouseInput::OnStandardMouseMoved });
			m_mouse.MouseMoved({ this, &MouseInput::OnRelativeMouseMoved });
		}

		void RelativeTrackingEnter()
		{
			if (!m_inRelativeTracking)
			{
				m_inRelativeTracking = true;
				m_originalCursor = m_window.PointerCursor();
				m_window.PointerCursor(nullptr);
			}
		}

		void RelativeTrackingExit()
		{
			if (m_inRelativeTracking)
			{
				m_inRelativeTracking = false;
				m_window.PointerCursor(m_originalCursor);
			}
		}

		float RelativeDeltaX()
		{
			float result = m_inRelativeTracking ? m_deltaX : 0;
			m_deltaX = 0;
			return result;
		}

		float RelativeDeltaY()
		{
			float result = m_inRelativeTracking ? m_deltaY : 0;
			m_deltaY = 0;
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
			if (m_inRelativeTracking)
			{
				m_deltaX += args.MouseDelta().X;
				m_deltaY += args.MouseDelta().Y;
			}
		}

		void OnStandardMouseMoved(CoreWindow window, PointerEventArgs args)
		{
			if (!m_inRelativeTracking)
			{
				m_pointerX = args.CurrentPoint().Position().X;
				m_pointerY = args.CurrentPoint().Position().Y;
			}
		}
		CoreCursor										m_originalCursor{ nullptr };
		CoreWindow										m_window;
		Windows::Devices::Input::MouseDevice	m_mouse;
		bool												m_inRelativeTracking = false;
		float												m_pointerX = 0;
		float												m_pointerY = 0;
		float												m_deltaX = 0;
		float												m_deltaY = 0;
	};
}