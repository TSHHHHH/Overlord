#pragma once

#include "Event.h"

namespace Overlord
{
	class OLD_API MouseMovedEvent : public Event
	{
	public:
		// Constructor
		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) { }

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(Mouse_Moved)
		EVENT_CLASS_CATEGORY(EventCategory_Mouse | EventCategory_Input)

	private:
		float m_MouseX, m_MouseY;
	};

	class OLD_API MouseScrolledEvent : public Event
	{
	public:
		// Constructor
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset)
		{
		}

		inline float GetXoffset() const { return m_XOffset; }
		inline float GetYoffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(Mouse_Scrolled)
		EVENT_CLASS_CATEGORY(EventCategory_Mouse | EventCategory_Input)

	private:
		float m_XOffset, m_YOffset;
	};

	// =============================================================
	// Base class
	class OLD_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategory_Mouse | EventCategory_Input)
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) { }

		int m_Button;
	};
	// =============================================================

	class OLD_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPresedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButton_Pressed)
	};

	class OLD_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) { }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButton_Released)
	};
}