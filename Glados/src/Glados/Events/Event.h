#pragma once
#include <string>

namespace Glados {

	// Events are currently blocking 

	// Current list of events
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// current list of event catecories using a bitfield
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

// macros for defining the event types and categories in each specific case
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	// base class for all events
	class Event
	{
		friend class EventDispatcher;
	public:
		bool Handled = false;
	public:
		// methods to be overriden in subclasses
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		// category filtering method
		inline bool IsInCategory(EventCategory c)
		{
			return GetCategoryFlags() & c;
		}
	};

	// used for handling events
	class EventDispatcher
	{

		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& e)
			: m_Event(e)
		{
		}
		
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (T::GetStaticType() == m_Event.GetEventType())
			{
				GD_CORE_ASSERT(T::GetStaticType() == m_Event.GetEventType(), "something's wrong");
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	// output stream override for printing events using the logger
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}