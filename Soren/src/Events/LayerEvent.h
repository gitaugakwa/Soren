#pragma once

#include "Event.h"

#include <string>
#include <sstream>

namespace Soren {
	// Status to display ID only and not name


	class SOREN_API LayerCreatedEvent : public Event
	{
	public:
		LayerCreatedEvent()
			 {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerCreatedEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerCreated)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		//const size_t m_ID;
		//const std::string m_Title;
	};

	class SOREN_API LayerDeletedEvent : public Event
	{
	public:
		LayerDeletedEvent()
			 {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerDeletedEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}
		EVENT_CLASS_TYPE(LayerDeleted)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		//const size_t m_ID;
		//const std::string m_Title;
	};

	class SOREN_API LayerResetEvent : public Event
	{
	public:
		LayerResetEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerResetEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			//ss << " (Nodes Reset -> " << m_Size << ")";
			return ss.str();
		}
		EVENT_CLASS_TYPE(LayerReset)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		//const size_t m_ID, m_Size;
		//const std::string m_Title;
	};

	class SOREN_API LayerDisabledEvent : public Event
	{
	public:
		LayerDisabledEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerDisabledEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}
		EVENT_CLASS_TYPE(LayerDisabled)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		//const size_t m_ID;
		//const std::string m_Title;
	};

	class SOREN_API LayerEnabledEvent : public Event
	{
	public:
		LayerEnabledEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerEnabledEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}
		EVENT_CLASS_TYPE(LayerEnabled)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		//const size_t m_ID;
		//const std::string m_Title;
	};

	class SOREN_API LayerClearedEvent : public Event
	{
	public:
		LayerClearedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerClearedEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			//ss << " (Nodes Cleared -> " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerCleared)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		//const size_t mId;
		//const std::string m_Title;
	};

	class SOREN_API LayerCopiedEvent : public Event
	{
	public:
		LayerCopiedEvent(size_t inId, size_t outId)
			: mInId(inId), mOutId(outId) {}


		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerCopiedEvent: ";
			ss << "[" << mInId << "]" << " -> " << "[" << mOutId << "]";
			//ss << m_Title << "[" << m_ID << "]";
			//ss << " (Size: " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerCopied)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t mInId, mOutId;
		//const std::string m_Title;
	};

	class SOREN_API LayerCopyAssignedEvent : public Event
	{
	public:
		LayerCopyAssignedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerCopyAssignedEvent: ";
			//ss << m_InTitle << "[" << m_InID << "]" << " <- " << m_OutTitle << "[" << m_OutID << "]";
			//ss << " (Size: " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerCopyAssigned)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		//const size_t m_OutID, m_InID, m_Size;
		//const std::string m_OutTitle, m_InTitle;
	};

	class SOREN_API LayerMovedEvent : public Event
	{
	public:
		LayerMovedEvent(size_t id)
			: mId(id) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerMovedEvent: ";
			ss << "[" << mId << "]";
			//ss << m_Title << "[" << m_ID << "]";
			//ss << " (Size: " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerMoved)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t mId;
		//const std::string m_Title;
	};

	class SOREN_API LayerMoveAssignedEvent : public Event
	{
	public:
		LayerMoveAssignedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerMoveAssignedEvent: ";
			//ss << m_InTitle << "[" << m_InID << "]" << " <- " << m_OutTitle << "[" << m_OutID << "]";
			//ss << " (Size: " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerMoveAssigned)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		//const size_t m_OutID, m_InID, m_Size;
		//const std::string m_OutTitle, m_InTitle;
	};

	class SOREN_API LayerActiveEvent : public Event
	{
	public:
		LayerActiveEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerActiveEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerActive)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		//const size_t m_ID;
		//const std::string m_Title;
	};

	class SOREN_API LayerInactiveEvent : public Event
	{
	public:
		LayerInactiveEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerInactiveEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerInactive)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		//const size_t m_ID;
		//const std::string m_Title;
	};

	//class SOREN_API LayerStatusChangeEvent : public Event
	//{
	//public:
	//	LayerStatusChangeEvent(const size_t id, const std::string& title, const std::string& prev, const std::string& curr)
	//		: m_Prev(prev), m_Curr(curr), m_Title(title), m_ID(id) {}

	//	std::string ToString() const override
	//	{
	//		std::stringstream ss;
	//		ss << "LayerStatusChangeEvent: ";
	//		ss << m_Title << "[" << m_ID << "]";
	//		ss << " (Previous State: " << m_Prev << " -> Current State: " << m_Curr << ")";
	//		return ss.str();
	//	}

	//	EVENT_CLASS_TYPE(LayerStatusChange)
	//	EVENT_CLASS_CATEGORY(EventCategoryLayer)

	//private:
	//	const std::string m_Prev, m_Curr, m_Title;
	//	const size_t m_ID;
	//};

}
