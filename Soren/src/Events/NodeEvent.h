#pragma once

#include "Event.h"

#include <string>
#include <sstream>

namespace Soren {



	class SOREN_API NodeCreatedEvent : public Event
	{
	public:
		NodeCreatedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NodeCreatedEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			//ss << " LayerID[" << m_LayerID << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(NodeCreated)
		EVENT_CLASS_CATEGORY(EventCategoryNode)

	private:
		//const size_t m_ID, m_LayerID;
		//const std::string m_Title;
	};

	class SOREN_API NodeDeletedEvent : public Event
	{
	public:
		NodeDeletedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NodeDeletedEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			//ss << " LayerID[" << m_LayerID << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(NodeDeleted)
		EVENT_CLASS_CATEGORY(EventCategoryNode)

	private:
	};

	class SOREN_API NodeResetEvent : public Event
	{
	public:
		NodeResetEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NodeResetEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			//ss << " (Links Reset -> " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(NodeReset)
		EVENT_CLASS_CATEGORY(EventCategoryNode)

	private:
		//const size_t m_ID, m_Size;
		//const std::string m_Title;
	};
	
	class SOREN_API NodeClearedEvent : public Event
	{
	public:
		NodeClearedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NodeClearedEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			//ss << " (Links Cleared -> " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(NodeCleared)
		EVENT_CLASS_CATEGORY(EventCategoryNode)

	private:
		//const size_t m_ID, m_Size;
		//const std::string m_Title;
	};
	
	class SOREN_API NodeDisabledEvent : public Event
	{
	public:
		NodeDisabledEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NodeDisabledEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(NodeDisabled)
		EVENT_CLASS_CATEGORY(EventCategoryNode)

	private:
		//const size_t m_ID;
		//const std::string m_Title;
	};

	class SOREN_API NodeEnabledEvent : public Event
	{
	public:
		NodeEnabledEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NodeEnabledEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(NodeEnabled)
		EVENT_CLASS_CATEGORY(EventCategoryNode)

	private:
		//const size_t m_ID;
		//const std::string m_Title;
	};
	

	class SOREN_API NodeCopiedEvent : public Event
	{
	public:
		NodeCopiedEvent(size_t inId, size_t outId)
			: mInId(inId), mOutId(outId) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NodeCopiedEvent: ";
			ss << "[" << mInId << "]" << " -> " << "[" << mOutId << "]";
			//ss << m_Title << "[" << m_ID << "]";
			//ss << " (Size: " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(NodeCopied)
		EVENT_CLASS_CATEGORY(EventCategoryNode)
	private:
		const size_t mInId,mOutId;
		//const std::string m_Title;
	};

	class SOREN_API NodeCopyAssignedEvent : public Event
	{
	public:
		NodeCopyAssignedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NodeCopyAssignedEvent: ";
			//ss << m_InTitle << "[" << m_InID << "]" << " <- " << m_OutTitle << "[" << m_OutID << "]";
			//ss << " (Size: " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(NodeCopyAssigned)
		EVENT_CLASS_CATEGORY(EventCategoryNode)
	private:
		//const size_t m_OutID, m_InID, m_Size;
		//const std::string m_OutTitle, m_InTitle;
	};

	class SOREN_API NodeMovedEvent : public Event
	{
	public:
		NodeMovedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NodeMovedEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			//ss << " (Size: " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(NodeMoved)
		EVENT_CLASS_CATEGORY(EventCategoryNode)
	private:
		//const size_t m_ID, m_Size;
		//const std::string m_Title;
	};

	class SOREN_API NodeMoveAssignedEvent : public Event
	{
	public:
		NodeMoveAssignedEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NodeMoveAssignedEvent: ";
			//ss << m_InTitle << "[" << m_InID << "]" << " <- " << m_OutTitle << "[" << m_OutID << "]";
			//ss << " (Size: " << m_Size << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(NodeMoveAssigned)
		EVENT_CLASS_CATEGORY(EventCategoryNode)
	private:
		//const size_t m_OutID, m_InID, m_Size;
		//const std::string m_OutTitle, m_InTitle;
	};

	class SOREN_API NodeActiveEvent : public Event
	{
	public:
		NodeActiveEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NodeActiveEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(NodeActive)
		EVENT_CLASS_CATEGORY(EventCategoryNode)

	private:
		//const size_t m_ID;
		//const std::string m_Title;
	};

	class SOREN_API NodeInactiveEvent : public Event
	{
	public:
		NodeInactiveEvent()
			{}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "NodeInactiveEvent: ";
			//ss << m_Title << "[" << m_ID << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(NodeInactive)
		EVENT_CLASS_CATEGORY(EventCategoryNode)

	private:
		//const size_t m_ID;
		//const std::string m_Title;
	};

	//class SOREN_API NodeStatusChangeEvent : public Event
	//{
	//public:
	//	NodeStatusChangeEvent(const size_t id, const std::string& name, const std::string& prev, const std::string& curr)
	//		: m_Prev(prev), m_Curr(curr), m_Title(name), m_ID(id) {}

	//	std::string ToString() const override
	//	{
	//		std::stringstream ss;
	//		ss << "NodeStatusChangeEvent: ";
	//		ss << m_Title << "[" << m_ID << "]";
	//		ss << " (Previous State: " << m_Prev << " -> Current State: " << m_Curr << ")";
	//		return ss.str();
	//	}

	//	EVENT_CLASS_TYPE(NodeStatusChange)
	//	EVENT_CLASS_CATEGORY(EventCategoryNode)

	//private:
	//	const std::string m_Prev, m_Curr, m_Title;
	//	const size_t m_ID;
	//};
	
}
