#pragma once

#include "Event.h"

#include <sstream>
#include <string>

namespace Soren {

	class SOREN_API OutputCreatedEvent : public Event
	{
	public:
		OutputCreatedEvent(unsigned int id, size_t width, size_t height)
			: m_ID(id), m_Width(width), m_Height(height) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputCreatedEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputCreated)
		EVENT_CLASS_CATEGORY(EventCategoryOutputData)

	private:
		unsigned int m_ID;
		size_t m_Width, m_Height;
	};

	class SOREN_API OutputDeletedEvent : public Event
	{
	public:
		OutputDeletedEvent(unsigned int id, size_t width, size_t height)
			: m_ID(id), m_Width(width), m_Height(height) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputDeletedEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputDeleted)
		EVENT_CLASS_CATEGORY(EventCategoryOutputData)

	private:
		unsigned int m_ID;
		size_t m_Width, m_Height;
	};

	class SOREN_API OutputClearedEvent : public Event
	{
	public:
		OutputClearedEvent(unsigned int id, size_t width, size_t height, size_t size)
			: m_ID(id), m_Width(width), m_Height(height), m_Size(size) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputClearedEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			ss << " Size: " << m_Size;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputCleared)
		EVENT_CLASS_CATEGORY(EventCategoryOutputData)

	private:
		unsigned int m_ID;
		size_t m_Size, m_Width, m_Height;
	};

	class SOREN_API OutputResetEvent : public Event
	{
	public:
		OutputResetEvent(unsigned int id, size_t width, size_t height, size_t size)
			: m_ID(id), m_Width(width), m_Height(height), m_Size(size) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputResetEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			ss << " Size: " << m_Size;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputReset)
		EVENT_CLASS_CATEGORY(EventCategoryOutputData)

	private:
		unsigned int m_ID;
		size_t m_Size, m_Width, m_Height;
	};

	class SOREN_API OutputDisabledEvent : public Event
	{
	public:
		OutputDisabledEvent(unsigned int id, size_t width, size_t height)
			: m_ID(id), m_Width(width), m_Height(height) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputDisabledEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputDisabled)
		EVENT_CLASS_CATEGORY(EventCategoryOutputData)

	private:
		unsigned int m_ID;
		size_t m_Width, m_Height;
	};

	class SOREN_API OutputEnabledEvent : public Event
	{
	public:
		OutputEnabledEvent(unsigned int id, size_t width, size_t height)
			: m_ID(id), m_Width(width), m_Height(height) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputEnabledEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputEnabled)
		EVENT_CLASS_CATEGORY(EventCategoryOutputData)

	private:
		unsigned int m_ID;
		size_t m_Width, m_Height;
	};

	class SOREN_API OutputActiveEvent : public Event
	{
	public:
		OutputActiveEvent(unsigned int id, size_t width, size_t height)
			: m_ID(id), m_Width(width), m_Height(height) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputActiveEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputActive)
		EVENT_CLASS_CATEGORY(EventCategoryOutputData)

	private:
		unsigned int m_ID;
		size_t m_Width, m_Height;
	};

	class SOREN_API OutputInactiveEvent : public Event
	{
	public:
		OutputInactiveEvent(unsigned int id, size_t width, size_t height)
			: m_ID(id), m_Width(width), m_Height(height) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputInactiveEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputInactive)
		EVENT_CLASS_CATEGORY(EventCategoryOutputData)

	private:
		unsigned int m_ID;
		size_t m_Width, m_Height;
	};

	class SOREN_API OutputStatusChangeEvent : public Event
	{
	public:
		OutputStatusChangeEvent(unsigned int id, size_t width, size_t height, const std::string& prev, const std::string& curr)
			: m_ID(id), m_Width(width), m_Height(height), m_Prev(prev), m_Curr(curr) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputStatusChangeEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			ss << " (Previous State: " << m_Prev << " -> Current State: " << m_Curr << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputStatusChange)
		EVENT_CLASS_CATEGORY(EventCategoryOutputData)

	private:
		std::string m_Prev, m_Curr;
		unsigned int m_ID;
		size_t m_Width, m_Height;
	};

	class SOREN_API OutputBufferCreatedEvent : public Event
	{
	public:
		OutputBufferCreatedEvent(unsigned int id)
			: m_ID(id) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputBufferCreatedEvent: ";
			ss << "ID[" << m_ID << "]";
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputBufferCreated)
		EVENT_CLASS_CATEGORY(EventCategoryOutputBuffer)

	private:
		unsigned int m_ID;
	};

	class SOREN_API OutputBufferDeletedEvent : public Event
	{
	public:
		OutputBufferDeletedEvent(unsigned int id, size_t width)
			: m_ID(id), m_Width(width) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputBufferDeletedEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputBufferDeleted)
		EVENT_CLASS_CATEGORY(EventCategoryOutputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Width;
	};

	class SOREN_API OutputBufferClearedEvent : public Event
	{
	public:
		OutputBufferClearedEvent(unsigned int id, size_t width, size_t size)
			: m_ID(id), m_Width(width), m_Size(size) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputBufferClearedEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			ss << " Size: " << m_Size;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputBufferCleared)
		EVENT_CLASS_CATEGORY(EventCategoryOutputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Size, m_Width;
	};

	class SOREN_API OutputBufferResetEvent : public Event
	{
	public:
		OutputBufferResetEvent(unsigned int id, size_t width, size_t size)
			: m_ID(id), m_Width(width), m_Size(size) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputBufferClearedEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			ss << " Size: " << m_Size;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputBufferReset)
		EVENT_CLASS_CATEGORY(EventCategoryOutputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Size, m_Width;
	};

	class SOREN_API OutputBufferDisabledEvent : public Event
	{
	public:
		OutputBufferDisabledEvent(unsigned int id, size_t width)
			: m_ID(id), m_Width(width) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputBufferDisabledEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputBufferDisabled)
		EVENT_CLASS_CATEGORY(EventCategoryOutputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Width;
	};

	class SOREN_API OutputBufferEnabledEvent : public Event
	{
	public:
		OutputBufferEnabledEvent(unsigned int id, size_t width)
			: m_ID(id), m_Width(width) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputBufferEnabledEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputBufferEnabled)
		EVENT_CLASS_CATEGORY(EventCategoryOutputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Width;
	};

	class SOREN_API OutputBufferOutputEvent : public Event
	{
	public:
		OutputBufferOutputEvent(unsigned int id, size_t width)
			: m_ID(id), m_Width(width) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputBufferRetrivedEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputBufferOutput)
		EVENT_CLASS_CATEGORY(EventCategoryOutputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Width;
	};

	class SOREN_API OutputBufferActiveEvent : public Event
	{
	public:
		OutputBufferActiveEvent(unsigned int id, size_t width)
			: m_ID(id), m_Width(width) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputBufferActiveEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputBufferActive)
		EVENT_CLASS_CATEGORY(EventCategoryOutputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Width;
	};

	class SOREN_API OutputBufferInactiveEvent : public Event
	{
	public:
		OutputBufferInactiveEvent(unsigned int id, size_t width)
			: m_ID(id), m_Width(width) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputBufferInactiveEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputBufferInactive)
		EVENT_CLASS_CATEGORY(EventCategoryOutputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Width;
	};

	class SOREN_API OutputBufferStatusChangeEvent : public Event
	{
	public:
		OutputBufferStatusChangeEvent(unsigned int id, size_t width, const std::string& prev, const std::string& curr)
			: m_ID(id), m_Width(width), m_Prev(prev), m_Curr(curr) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "OutputBufferStatusChangeEvent: ";
			ss << "ID[" << m_ID << "]";
			ss << " Width: " << m_Width;
			ss << " (Previous State: " << m_Prev << " -> Current State: " << m_Curr << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(OutputBufferStatusChange)
		EVENT_CLASS_CATEGORY(EventCategoryOutputBuffer)

	private:
		std::string m_Prev, m_Curr;
		unsigned int m_ID;
		size_t m_Width;
	};


}
