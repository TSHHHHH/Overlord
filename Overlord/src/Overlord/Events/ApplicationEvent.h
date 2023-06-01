#pragma once

#include "Event.h"

namespace Overlord
{
	class OLD_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) { }

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}
		
		EVENT_CLASS_TYPE(Window_Resize)
		EVENT_CLASS_CATEGORY(EventCategory_Application)

	private:
		unsigned int m_Width, m_Height;
	};

	class OLD_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(Window_Close)
		EVENT_CLASS_CATEGORY(EventCategory_Application)
	};

	class OLD_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(App_Tick)
		EVENT_CLASS_CATEGORY(EventCategory_Application)
	};

	class OLD_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(App_Update)
		EVENT_CLASS_CATEGORY(EventCategory_Application)
	};

	class OLD_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(App_Render)
		EVENT_CLASS_CATEGORY(EventCategory_Application)
	};
}