#pragma once

#include "Event.h"
#include "Type.h"

#include <sstream>
#include <string>

namespace Soren {

	class SOREN_API LinkCreatedEvent : public Event
	{
	public:
		LinkCreatedEvent(const size_t id, const size_t in, const size_t out)
			: m_ID(id), m_InID(in), m_OutID(out) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LinkCreatedEvent: ";
			ss << "[" << m_ID << "]";
			ss << "NodeIDs: " << m_InID << " -> " << m_OutID;
			return ss.str();
		}

		EVENT_CLASS_TYPE(LinkCreated)
		EVENT_CLASS_CATEGORY(EventCategoryLink)

	private:
		const size_t m_ID, m_InID, m_OutID;
	};
	
	class SOREN_API LinkDeletedEvent : public Event
	{
	public:
		LinkDeletedEvent(const size_t id, const size_t in, const size_t out)
			: m_ID(id), m_InID(in), m_OutID(out) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LinkDeletedEvent: ";
			ss << "[" << m_ID << "]";
			ss << "NodeIDs: " << m_InID << " -> " << m_OutID;
			return ss.str();
		}

		EVENT_CLASS_TYPE(LinkDeleted)
		EVENT_CLASS_CATEGORY(EventCategoryLink)

	private:
		const size_t m_ID, m_InID, m_OutID;
	};

	class SOREN_API LinkResetEvent : public Event
	{
	public:
		LinkResetEvent(const size_t id, const size_t in, const size_t out, const Weight_t weight)
			: m_ID(id), m_InID(in), m_OutID(out), m_Weight(weight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LinkResetEvent: ";
			ss << "[" << m_ID << "]";
			ss << "NodeIDs: " << m_InID << " -> " << m_OutID;
			ss << " (Previous Weight: " << m_Weight << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(LinkReset)
		EVENT_CLASS_CATEGORY(EventCategoryLink)

	private:
		const size_t m_ID, m_InID, m_OutID;
		const Weight_t m_Weight;
	};

	class SOREN_API LinkClearedEvent : public Event
	{
	public:
		LinkClearedEvent(const size_t id, const size_t in, const size_t out, const Weight_t weight)
			: m_ID(id), m_InID(in), m_OutID(out), m_Weight(weight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LinkClearedEvent: ";
			ss << "[" << m_ID << "]";
			ss << "Weight: " << m_Weight;
			return ss.str();
		}

		EVENT_CLASS_TYPE(LinkCleared)
		EVENT_CLASS_CATEGORY(EventCategoryLink)

	private:
		const size_t m_ID, m_InID, m_OutID;
		const Weight_t m_Weight;
	};

	class SOREN_API LinkDisabledEvent : public Event
	{
	public:
		LinkDisabledEvent(const size_t id, const size_t in, const size_t out)
			: m_ID(id), m_InID(in), m_OutID(out) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LinkDisabledEvent: ";
			ss << "[" << m_ID << "]";
			ss << "NodeIDs: " << m_InID << " -> " << m_OutID;
			return ss.str();
		}

		EVENT_CLASS_TYPE(LinkDisabled)
		EVENT_CLASS_CATEGORY(EventCategoryLink)

	private:
		const size_t m_ID, m_InID, m_OutID;
	};

	class SOREN_API LinkEnabledEvent : public Event
	{
	public:
		LinkEnabledEvent(const size_t id, const size_t in, const size_t out)
			: m_ID(id), m_InID(in), m_OutID(out) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LinkEnabledEvent: ";
			ss << "[" << m_ID << "]";
			ss << "NodeIDs: " << m_InID << " -> " << m_OutID;
			return ss.str();
		}

		EVENT_CLASS_TYPE(LinkEnabled)
		EVENT_CLASS_CATEGORY(EventCategoryLink)

	private:
		const size_t m_ID, m_InID, m_OutID;
	};

	class SOREN_API LinkCopiedEvent : public Event
	{
	public:
		LinkCopiedEvent(const size_t id, const size_t in, const size_t out, const Weight_t weight)
			: m_ID(id), m_InID(in), m_OutID(out), m_Weight(weight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LinkCopiedEvent: ";
			ss << "[" << m_ID << "]";
			ss << "NodeIDs: " << m_InID << " -> " << m_OutID;
			ss << " Weight: " << m_Weight;
			return ss.str();
		}

		EVENT_CLASS_TYPE(LinkCopied)
		EVENT_CLASS_CATEGORY(EventCategoryLink)
	private:
		const size_t m_ID, m_InID, m_OutID;
		const Weight_t m_Weight;
	};

	class SOREN_API LinkCopyAssignedEvent : public Event
	{
	public:
		LinkCopyAssignedEvent(const size_t outid, const size_t inid, const size_t outinid, const size_t outoutid, const size_t ininid, const size_t inoutid, const Weight_t outweight, const Weight_t inweight)
			: m_OutID(outid), m_InID(inid), m_OutInID(outinid), m_OutOutID(outoutid), m_InInID(ininid), m_InOutID(inoutid), m_OutWeight(outweight), m_InWeight(inweight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LinkCopyAssignedEvent: ";
			ss  << "[" << m_InID << "]";
			ss << "NodeIDs: " << m_InOutID << " -> " << m_InInID;
			ss << " Weight: " << m_InWeight;
			ss << " <- " << "[" << m_OutID << "]";
			ss << "NodeIDs: " << m_OutOutID << " -> " << m_OutInID;
			ss << " Weight: " << m_OutWeight;
			return ss.str();
		}

		EVENT_CLASS_TYPE(LinkCopyAssigned)
		EVENT_CLASS_CATEGORY(EventCategoryLink)
	private:
		const size_t m_OutID, m_InID, m_OutInID, m_OutOutID, m_InInID, m_InOutID;
		const Weight_t m_OutWeight, m_InWeight;
	};

	class SOREN_API LinkMovedEvent : public Event
	{
	public:
		LinkMovedEvent(const size_t id, const size_t in, const size_t out, const Weight_t weight)
			: m_ID(id), m_InID(in), m_OutID(out), m_Weight(weight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LinkMovedEvent: ";
			ss << "[" << m_ID << "]";
			ss << "NodeIDs: " << m_InID << " -> " << m_OutID;
			ss << " Weight: " << m_Weight;
			return ss.str();
		}

		EVENT_CLASS_TYPE(LinkMoved)
		EVENT_CLASS_CATEGORY(EventCategoryLink)
	private:
		const size_t m_ID, m_InID, m_OutID;
		const Weight_t m_Weight;
	};

	class SOREN_API LinkMoveAssignedEvent : public Event
	{
	public:
		LinkMoveAssignedEvent(const size_t outid, const size_t inid, const size_t outinid, const size_t outoutid, const size_t ininid, const size_t inoutid, const Weight_t outweight, const Weight_t inweight)
			: m_OutID(outid), m_InID(inid), m_OutInID(outinid), m_OutOutID(outoutid), m_InInID(ininid), m_InOutID(inoutid), m_OutWeight(outweight), m_InWeight(inweight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "LinkMoveAssignedEvent: ";
			ss << "[" << m_InID << "]";
			ss << "NodeIDs: " << m_InOutID << " -> " << m_InInID;
			ss << " Weight: " << m_InWeight;
			ss << " <- " << "[" << m_OutID << "]";
			ss << "NodeIDs: " << m_OutOutID << " -> " << m_OutInID;
			ss << " Weight: " << m_OutWeight;
			return ss.str();
		}

		EVENT_CLASS_TYPE(LinkMoveAssigned)
		EVENT_CLASS_CATEGORY(EventCategoryLink)
	private:
		const size_t m_OutID, m_InID, m_OutInID, m_OutOutID, m_InInID, m_InOutID;
		const Weight_t m_OutWeight, m_InWeight;
	};

	class SOREN_API LinkActiveEvent : public Event
	{
	public:
		LinkActiveEvent(const size_t id, const size_t in, const size_t out)
			: m_ID(id), m_InID(in), m_OutID(out) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[" << m_ID << "]";
			ss << "LinkActiveEvent: ";
			ss << "NodeIDs: " << m_InID << " -> " << m_OutID;
			return ss.str();
		}

		EVENT_CLASS_TYPE(LinkActive)
		EVENT_CLASS_CATEGORY(EventCategoryLink)

	private:
		const size_t m_ID, m_InID, m_OutID;
	};

	class SOREN_API LinkInactiveEvent : public Event
	{
	public:
		LinkInactiveEvent(const size_t id, const size_t in, const size_t out)
			: m_ID(id), m_InID(in), m_OutID(out) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "[" << m_ID << "]";
			ss << "LinkInactiveEvent: ";
			ss << "NodeIDs: " << m_InID << " -> " << m_OutID;
			return ss.str();
		}

		EVENT_CLASS_TYPE(LinkInactive)
		EVENT_CLASS_CATEGORY(EventCategoryLink)

	private:
		const size_t m_ID, m_InID, m_OutID;
	};

	//class SOREN_API LinkStatusChangeEvent : public Event
	//{
	//public:
	//	LinkStatusChangeEvent(const size_t id, const size_t in, const size_t out, cconst std::string& prev, const std::string& curr)
	//		: m_ID(id), m_Prev(prev), m_Curr(curr), m_InID(in), m_OutID(out) {}

	//	std::string ToString() const override
	//	{
	//		std::stringstream ss;
	//		ss << "LinkStatusChangeEvent: ";
	//		ss << "[" << m_ID << "]";
	//		ss << "NodeIDs: " << m_InID << " -> " << m_OutID;
	//		ss << " (Previous State: " << m_Prev << " -> Current State: " << m_Curr << ")";
	//		return ss.str();
	//	}

	//	EVENT_CLASS_TYPE(LinkStatusChange)
	//	EVENT_CLASS_CATEGORY(EventCategoryLink)

	//private:
	//	conststd::string m_Prev, m_Curr;
	//	constsize_t m_ID, m_InID, m_OutID;
	//};

}
