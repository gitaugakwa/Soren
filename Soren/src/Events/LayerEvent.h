#pragma once

#include "Event.h"

#include <sstream>

namespace Soren {
	// Status to display ID only and not name


	class SOREN_API LayerCreatedEvent : public Event
	{
	public:
		LayerCreatedEvent(const size_t id, const std::string& title)
			: m_ID(id), m_Title(title) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerCreatedEvent: ";
			ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerCreated)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t m_ID;
		const std::string m_Title;
	};

	class SOREN_API LayerDeletedEvent : public Event
	{
	public:
		LayerDeletedEvent(const size_t id, const std::string& title)
			: m_ID(id), m_Title(title) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerDeletedEvent: ";
			ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}
		EVENT_CLASS_TYPE(LayerDeleted)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t m_ID;
		const std::string m_Title;
	};

	class SOREN_API LayerResetEvent : public Event
	{
	public:
		LayerResetEvent(const size_t id, const std::string& title, const size_t size)
			: m_ID(id), m_Size(size), m_Title(title) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerResetEvent: ";
			ss << m_Title << "[" << m_ID << "]";
			ss << " (Nodes Reset -> " << m_Size << ")";
			return ss.str();
		}
		EVENT_CLASS_TYPE(LayerReset)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t m_ID, m_Size;
		const std::string m_Title;
	};

	class SOREN_API LayerDisabledEvent : public Event
	{
	public:
		LayerDisabledEvent(const size_t id, const std::string& title)
			: m_ID(id), m_Title(title) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerDisabledEvent: ";
			ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}
		EVENT_CLASS_TYPE(LayerDisabled)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t m_ID;
		const std::string m_Title;
	};

	class SOREN_API LayerEnabledEvent : public Event
	{
	public:
		LayerEnabledEvent(const size_t id, const std::string& title)
			: m_ID(id), m_Title(title) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerEnabledEvent: ";
			ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}
		EVENT_CLASS_TYPE(LayerEnabled)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t m_ID;
		const std::string m_Title;
	};

	class SOREN_API LayerClearedEvent : public Event
	{
	public:
		LayerClearedEvent(const size_t id, const std::string& title, const size_t size)
		: m_ID(id), m_Size(size), m_Title(title) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerClearedEvent: ";
			ss << m_Title << "[" << m_ID << "]";
			ss << " (Nodes Cleared -> " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerCleared)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t m_ID, m_Size;
		const std::string m_Title;
	};

	class SOREN_API LayerCopiedEvent : public Event
	{
	public:
		LayerCopiedEvent(const size_t id, const std::string& title, const size_t size)
		: m_ID(id), m_Size(size), m_Title(title) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerCopiedEvent: ";
			ss << m_Title << "[" << m_ID << "]";
			ss << " (Size: " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerCopied)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t m_ID, m_Size;
		const std::string m_Title;
	};

	class SOREN_API LayerCopyAssignedEvent : public Event
	{
	public:
		LayerCopyAssignedEvent(const size_t outid, const size_t inid, const std::string& outtitle, const std::string& intitle, const size_t size)
		: m_OutID(outid), m_InID(inid), m_Size(size), m_OutTitle(outtitle), m_InTitle(intitle) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerCopyAssignedEvent: ";
			ss << m_InTitle << "[" << m_InID << "]" << " <- " << m_OutTitle << "[" << m_OutID << "]";
			ss << " (Size: " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerCopyAssigned)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t m_OutID, m_InID, m_Size;
		const std::string m_OutTitle, m_InTitle;
	};

	class SOREN_API LayerMovedEvent : public Event
	{
	public:
		LayerMovedEvent(const size_t id, const std::string& title, const size_t size)
		: m_ID(id), m_Size(size), m_Title(title) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerMovedEvent: ";
			ss << m_Title << "[" << m_ID << "]";
			ss << " (Size: " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerMoved)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t m_ID, m_Size;
		const std::string m_Title;
	};

	class SOREN_API LayerMoveAssignedEvent : public Event
	{
	public:
		LayerMoveAssignedEvent(const size_t outid, const size_t inid, const std::string& outtitle, const std::string& intitle, const size_t size)
			: m_OutID(outid), m_InID(inid), m_Size(size), m_OutTitle(outtitle), m_InTitle(intitle) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerMoveAssignedEvent: ";
			ss << m_InTitle << "[" << m_InID << "]" << " <- " << m_OutTitle << "[" << m_OutID << "]";
			ss << " (Size: " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerMoveAssigned)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t m_OutID, m_InID, m_Size;
		const std::string m_OutTitle, m_InTitle;
	};

	class SOREN_API LayerActiveEvent : public Event
	{
	public:
		LayerActiveEvent(const size_t id, const std::string& title)
			: m_ID(id), m_Title(title) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerActiveEvent: ";
			ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerActive)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t m_ID;
		const std::string m_Title;
	};

	class SOREN_API LayerInactiveEvent : public Event
	{
	public:
		LayerInactiveEvent(const size_t id, const std::string& title)
			: m_ID(id), m_Title(title) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LayerInactiveEvent: ";
			ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LayerInactive)
		EVENT_CLASS_CATEGORY(EventCategoryLayer)
	private:
		const size_t m_ID;
		const std::string m_Title;
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
