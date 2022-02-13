#pragma once

#include "Core.h"
#include "Type.h"

#include <string>

namespace Soren {

	// Events in Soren are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then and there.
	// For the future, a better strategy might be to buffer evens in an event
	// bus and process them during the "event" part of the update stage.

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,

		// Input // maybe Input/Output Initialized
		InputCreated, InputDeleted, InputCleared, InputReset, InputDisabled, InputEnabled, InputActive, InputInactive, InputStatusChange,
		InputBufferCreated, InputBufferDeleted, InputBufferCleared, InputBufferReset, InputBufferDisabled, InputBufferEnabled, InputBufferRetrieved, InputBufferActive, InputBufferInactive, InputBufferStatusChange,

		// Output
		OutputCreated, OutputDeleted, OutputCleared, OutputReset, OutputDisabled, OutputEnabled, OutputActive, OutputInactive, OutputStatusChange,
		OutputBufferCreated, OutputBufferDeleted, OutputBufferCleared, OutputBufferReset, OutputBufferDisabled, OutputBufferEnabled, OutputBufferOutput, OutputBufferActive, OutputBufferInactive, OutputBufferStatusChange,

		// Remember to add custom functions in respective files
		NetworkInitialized, NetworkCreated, NetworkDeleted, NetworkReset, NetworkDisabled, NetworkEnabled, NetworkCleared, NetworkCopied, NetworkCopyAssigned, NetworkMoved, NetworkMoveAssigned, NetworkActive, NetworkInactive, NetworkIterationStart, NetworkIterationStop, NetworkStatusChange,
		LayerCreated, LayerDeleted, LayerReset, LayerDisabled, LayerEnabled, LayerCleared, LayerCopied, LayerCopyAssigned, LayerMoved, LayerMoveAssigned, LayerActive, LayerInactive, LayerStatusChange,
		NodeCreated, NodeDeleted, NodeReset, NodeCleared, NodeDisabled, NodeEnabled, NodeCopied, NodeCopyAssigned, NodeMoved, NodeMoveAssigned, NodeActive, NodeInactive, NodeStatusChange,
		LinkCreated, LinkDeleted, LinkReset, LinkCleared, LinkDisabled, LinkEnabled, LinkCopied, LinkCopyAssigned, LinkMoved, LinkMoveAssigned, LinkActive, LinkInactive, LinkStatusChange
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	=BIT(0),
		EventCategoryInput			=BIT(1),
		EventCategoryKeyboard		=BIT(2),
		EventCategoryMouse			=BIT(3),
		EventCategoryMouseButton	=BIT(4),

		EventCategoryInputData		=BIT(5),
		EventCategoryInputBuffer	=BIT(6),

		EventCategoryOutputData		=BIT(7),
		EventCategoryOutputBuffer	=BIT(8),

		EventCategoryNetwork		=BIT(9),
		EventCategoryLayer			=BIT(10),
		EventCategoryNode			=BIT(11),
		EventCategoryLink			=BIT(12)
	};

#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)	virtual int GetCategoryFlags() const override { return category; }

	class SOREN_API Event
	{
		friend class EventDispatcher;
	public:

		mutable bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(const Event& event)
			: m_Event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		const Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
