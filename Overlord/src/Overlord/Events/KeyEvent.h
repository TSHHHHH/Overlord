#pragma once

#include "Event.h"

namespace Overlord
{
	// =============================================================
	// Base class
	class OLD_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategory_Keyboard | EventCategory_Input)

	protected:
		// Constructor
		KeyEvent(int keycode)
			: m_KeyCode(keycode) { }

		int m_KeyCode;
	};
	// =============================================================

	class OLD_API KeyPressedEvent : public KeyEvent
	{
	public:
		// Constructor
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) { }

		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(Key_Pressed)

	private:
		int m_RepeatCount;
	};

	class OLD_API KeyReleasedEvent : public KeyEvent
	{
	public:
		// Constructor
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(Key_Released)
	};
}
