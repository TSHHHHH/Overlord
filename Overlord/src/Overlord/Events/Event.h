#pragma once

#include "oldpch.h"
#include "Overlord/core.h"

namespace Overlord
{
	enum class EventType
	{
		None = 0,
		Window_Close, Window_Resize, Window_Focus, Window_LostFocus, Window_Moved,
		App_Tick, App_Update, App_Render,
		Key_Pressed, Key_Released, Key_Typed,
		MouseButton_Pressed, MouseButton_Released, Mouse_Moved, Mouse_Scrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategory_Application	= BIT(0),
		EventCategory_Input			= BIT(1),
		EventCategory_Keyboard		= BIT(2),
		EventCategory_Mouse			= BIT(3),
		EventCategory_MouseButton	= BIT(4)
	};

#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class OLD_API Event
	{
		friend class EventDispatcher;

	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool isHandled() const { return m_Handled; }
		bool& GetHandled() { return m_Handled; }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
		
	protected:
		bool m_Handled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: m_Event(event) { }

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}