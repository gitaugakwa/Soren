#pragma once

#include "Event.h"

#include <string>
#include <sstream>

namespace Soren {
	class SOREN_API NetworkInitializedEvent : public Event
	{
	public:
		NetworkInitializedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkInitializedEvent: ";
			//ss << m_Title;
			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkInitialized)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const std::string m_Title;
	};


	class SOREN_API NetworkCreatedEvent : public Event
	{
	public:
		NetworkCreatedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkCreatedEvent: ";
			//ss << m_Title;
			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkCreated)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const std::string m_Title;
	};

	class SOREN_API NetworkDeletedEvent : public Event
	{
	public:
		NetworkDeletedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkClosedEvent: ";
			//ss << m_Title;


			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkDeleted)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const std::string m_Title;
	};

	class SOREN_API NetworkResetEvent : public Event
	{
	public:
		NetworkResetEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkResetEvent: ";
			//ss << m_Title;
			//ss << " (Layers Reset -> " << m_Size << ")";

			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkReset)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const std::string m_Title;
		//const size_t m_Size;
	};

	class SOREN_API NetworkDisabledEvent : public Event
	{
	public:
		NetworkDisabledEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkDisabledEvent: ";
			//ss << m_Title;

			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkDisabled)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const std::string m_Title;
	};

	class SOREN_API NetworkEnabledEvent : public Event
	{
	public:
		NetworkEnabledEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkEnabledEvent: ";
			//ss << m_Title;

			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkEnabled)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const std::string m_Title;
	};

	class SOREN_API NetworkClearedEvent : public Event
	{
	public:
		NetworkClearedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkClearedEvent: ";
			//ss << m_Title;
			//ss << " (Layers Cleared -> " << m_Size << ")";
			
			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkCleared)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const size_t m_Size;
		//const std::string m_Title;
	};
	
	class SOREN_API NetworkCopiedEvent : public Event
	{
	public:
		NetworkCopiedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkCopiedEvent: ";
			//ss << m_Title;
			//ss << " (Size: " << m_Size << ")";
			
			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkCopied)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const size_t m_Size;
		//const std::string m_Title;
	};
	
	class SOREN_API NetworkCopyAssignedEvent : public Event
	{
	public:
		NetworkCopyAssignedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkCopyAssignedEvent: ";
			//ss << m_InTitle << " <- " << m_OutTitle;
			//ss << " (Size: " << m_Size << ")";
			
			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkCopyAssigned)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const size_t m_Size;
		//const std::string m_OutTitle, m_InTitle;
	};


	class SOREN_API NetworkMovedEvent : public Event
	{
	public:
		NetworkMovedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkMovedEvent: ";
			//ss << m_Title;
			//ss << " (Size: " << m_Size << ")";

			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkMoved)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const size_t m_Size;
		//const std::string m_Title;
	};

	class SOREN_API NetworkMoveAssignedEvent : public Event
	{
	public:
		NetworkMoveAssignedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkMoveAssignedEvent: ";
			//ss << m_InTitle << " <- " << m_OutTitle;
			//ss << " (Size: " << m_Size << ")";

			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkMoveAssigned)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const size_t m_Size;
		//const std::string m_OutTitle, m_InTitle;
	};

	class SOREN_API NetworkActiveEvent : public Event
	{
	public:
		NetworkActiveEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkActiveEvent: ";
			//ss << m_Title;

			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkActive)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const std::string m_Title;
	};

	class SOREN_API NetworkInactiveEvent : public Event
	{
	public:
		NetworkInactiveEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkInactiveEvent: ";
			//ss << m_Title;

			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkInactive)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const std::string m_Title;
	};

	class SOREN_API NetworkIterationStartEvent : public Event
	{
	public:
		NetworkIterationStartEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkIterationStartEvent: ";
			//ss << m_Title;
				
			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkIterationStart)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const std::string m_Title;
	};

	class SOREN_API NetworkIterationStopEvent : public Event
	{
	public:
		NetworkIterationStopEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkIterationStopEvent: ";
			//ss << m_Title;
				
			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkIterationStop)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const std::string m_Title;
	};

	class SOREN_API NetworkStatusChangeEvent : public Event
	{
	public:
		NetworkStatusChangeEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NetworkStatusChangeEvent: ";
			//ss << m_Title;
			//ss << " (Previous State: " << m_Prev << " -> Current State: " << m_Curr << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(NetworkStatusChange)
		EVENT_CLASS_CATEGORY(EventCategoryNetwork)

	private:
		//const std::string m_Title, m_Prev, m_Curr;
	};

}
