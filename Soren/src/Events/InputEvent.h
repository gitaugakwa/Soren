#pragma once

#include "Event.h"

#include <sstream>
#include <string>

namespace Soren {

	class SOREN_API InputCreatedEvent : public Event
	{
	public:
		InputCreatedEvent(unsigned int id, size_t width, size_t height, size_t datawidth, size_t dataheight)
			: m_ID(id), m_Width(width), m_Height(height), m_DataWidth(datawidth), m_DataHeight(dataheight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputCreatedEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputCreated)
		EVENT_CLASS_CATEGORY(EventCategoryInputData)

	private:
		unsigned int m_ID;
		size_t m_Width, m_Height, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputDeletedEvent : public Event
	{
	public:
		InputDeletedEvent(unsigned int id, size_t width, size_t height, size_t datawidth, size_t dataheight)
			: m_ID(id), m_Width(width), m_Height(height), m_DataWidth(datawidth), m_DataHeight(dataheight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputDeletedEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputDeleted)
		EVENT_CLASS_CATEGORY(EventCategoryInputData)

	private:
		unsigned int m_ID;
		size_t m_Width, m_Height, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputClearedEvent : public Event
	{
	public:
		InputClearedEvent(unsigned int id, size_t width, size_t height, size_t datawidth, size_t dataheight, size_t size)
			: m_ID(id), m_Width(width), m_Height(height), m_DataWidth(datawidth), m_DataHeight(dataheight), m_Size(size) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputClearedEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			ss << " Size: " << m_Size;
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputCleared)
		EVENT_CLASS_CATEGORY(EventCategoryInputData)

	private:
		unsigned int m_ID;
		size_t m_Size, m_Width, m_Height, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputResetEvent : public Event
	{
	public:
		InputResetEvent(unsigned int id, size_t width, size_t height, size_t datawidth, size_t dataheight, size_t size)
			: m_ID(id), m_Width(width), m_Height(height), m_DataWidth(datawidth), m_DataHeight(dataheight), m_Size(size) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputResetEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			ss << " Size: " << m_Size;
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputReset)
		EVENT_CLASS_CATEGORY(EventCategoryInputData)

	private:
		unsigned int m_ID;
		size_t m_Size, m_Width, m_Height, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputDisabledEvent : public Event
	{
	public:
		InputDisabledEvent(unsigned int id, size_t width, size_t height, size_t datawidth, size_t dataheight)
			: m_ID(id), m_Width(width), m_Height(height), m_DataWidth(datawidth), m_DataHeight(dataheight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputDisabledEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputDisabled)
		EVENT_CLASS_CATEGORY(EventCategoryInputData)

	private:
		unsigned int m_ID;
		size_t m_Width, m_Height, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputEnabledEvent : public Event
	{
	public:
		InputEnabledEvent(unsigned int id, size_t width, size_t height, size_t datawidth, size_t dataheight)
			: m_ID(id), m_Width(width), m_Height(height), m_DataWidth(datawidth), m_DataHeight(dataheight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputEnabledEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputEnabled)
		EVENT_CLASS_CATEGORY(EventCategoryInputData)

	private:
		unsigned int m_ID;
		size_t m_Width, m_Height, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputActiveEvent : public Event
	{
	public:
		InputActiveEvent(unsigned int id, size_t width, size_t height, size_t datawidth, size_t dataheight)
			: m_ID(id), m_Width(width), m_Height(height), m_DataWidth(datawidth), m_DataHeight(dataheight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputActiveEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputActive)
		EVENT_CLASS_CATEGORY(EventCategoryInputData)

	private:
		unsigned int m_ID;
		size_t m_Width, m_Height, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputInactiveEvent : public Event
	{
	public:
		InputInactiveEvent(unsigned int id, size_t width, size_t height, size_t datawidth, size_t dataheight)
			: m_ID(id), m_Width(width), m_Height(height), m_DataWidth(datawidth), m_DataHeight(dataheight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputInactiveEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputInactive)
		EVENT_CLASS_CATEGORY(EventCategoryInputData)

	private:
		unsigned int m_ID;
		size_t m_Width, m_Height, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputStatusChangeEvent : public Event
	{
	public:
		InputStatusChangeEvent(unsigned int id, size_t width, size_t height, size_t datawidth, size_t dataheight, const std::string& prev, const std::string& curr)
			: m_ID(id), m_Width(width), m_Height(height), m_DataWidth(datawidth), m_DataHeight(dataheight), m_Prev(prev), m_Curr(curr) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputStatusChangeEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			ss << " Height: " << m_Height;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			ss << " (Previous State: " << m_Prev << " -> Current State: " << m_Curr << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputStatusChange)
		EVENT_CLASS_CATEGORY(EventCategoryInputData)

	private:
		std::string m_Prev, m_Curr;
		unsigned int m_ID;
		size_t m_Width, m_Height, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputBufferCreatedEvent : public Event
	{
	public:
		InputBufferCreatedEvent(unsigned int id)
			: m_ID(id) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputBufferCreatedEvent: ";
			ss << "ID{" << m_ID << "}";
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputBufferCreated)
		EVENT_CLASS_CATEGORY(EventCategoryInputBuffer)

	private:
		unsigned int m_ID;
	};

	class SOREN_API InputBufferDeletedEvent : public Event
	{
	public:
		InputBufferDeletedEvent(unsigned int id, size_t width, size_t datawidth, size_t dataheight)
			: m_ID(id), m_Width(width), m_DataWidth(datawidth), m_DataHeight(dataheight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputBufferDeletedEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputBufferDeleted)
		EVENT_CLASS_CATEGORY(EventCategoryInputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Width, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputBufferClearedEvent : public Event
	{
	public:
		InputBufferClearedEvent(unsigned int id, size_t width, size_t datawidth, size_t dataheight, size_t size)
			: m_ID(id), m_Width(width), m_DataWidth(datawidth), m_DataHeight(dataheight), m_Size(size) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputBufferClearedEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			ss << " Size: " << m_Size;
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputBufferCleared)
		EVENT_CLASS_CATEGORY(EventCategoryInputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Size, m_Width, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputBufferResetEvent : public Event
	{
	public:
		InputBufferResetEvent(unsigned int id, size_t width, size_t datawidth, size_t dataheight, size_t size)
			: m_ID(id), m_Width(width), m_DataWidth(datawidth), m_DataHeight(dataheight), m_Size(size) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputBufferResetEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			ss << " Size: " << m_Size;
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputBufferReset)
		EVENT_CLASS_CATEGORY(EventCategoryInputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Size, m_Width, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputBufferDisabledEvent : public Event
	{
	public:
		InputBufferDisabledEvent(unsigned int id, size_t width, size_t datawidth, size_t dataheight)
			: m_ID(id), m_Width(width), m_DataWidth(datawidth), m_DataHeight(dataheight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputBufferDisabledEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputBufferDisabled)
		EVENT_CLASS_CATEGORY(EventCategoryInputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Width, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputBufferEnabledEvent : public Event
	{
	public:
		InputBufferEnabledEvent(unsigned int id, size_t width, size_t datawidth, size_t dataheight)
			: m_ID(id), m_Width(width), m_DataWidth(datawidth), m_DataHeight(dataheight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputBufferEnabledEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputBufferEnabled)
		EVENT_CLASS_CATEGORY(EventCategoryInputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Width, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputBufferRetrivedEvent : public Event
	{
	public:
		InputBufferRetrivedEvent(unsigned int id, size_t width, size_t datawidth, size_t dataheight)
			: m_ID(id), m_Width(width), m_DataWidth(datawidth), m_DataHeight(dataheight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputBufferRetrivedEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputBufferRetrieved)
		EVENT_CLASS_CATEGORY(EventCategoryInputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Width, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputBufferActiveEvent : public Event
	{
	public:
		InputBufferActiveEvent(unsigned int id, size_t width, size_t datawidth, size_t dataheight)
			: m_ID(id), m_Width(width), m_DataWidth(datawidth), m_DataHeight(dataheight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputBufferActiveEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputBufferActive)
		EVENT_CLASS_CATEGORY(EventCategoryInputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Width, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputBufferInactiveEvent : public Event
	{
	public:
		InputBufferInactiveEvent(unsigned int id, size_t width, size_t datawidth, size_t dataheight)
			: m_ID(id), m_Width(width), m_DataWidth(datawidth), m_DataHeight(dataheight) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputBufferInactiveEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputBufferInactive)
		EVENT_CLASS_CATEGORY(EventCategoryInputBuffer)

	private:
		unsigned int m_ID;
		size_t m_Width, m_DataWidth, m_DataHeight;
	};

	class SOREN_API InputBufferStatusChangeEvent : public Event
	{
	public:
		InputBufferStatusChangeEvent(unsigned int id, size_t width, size_t datawidth, size_t dataheight, const std::string& prev, const std::string& curr)
			: m_ID(id), m_Width(width), m_DataWidth(datawidth), m_DataHeight(dataheight), m_Prev(prev), m_Curr(curr) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "InputBufferStatusChangeEvent: ";
			ss << "ID{" << m_ID << "}";
			ss << " Width: " << m_Width;
			if (m_DataWidth)
			{
				ss << " DataWidth: " << m_DataWidth;
			}
			if (m_DataHeight)
			{
				ss << " DataHeight: " << m_DataHeight;
			}
			ss << " (Previous State: " << m_Prev << " -> Current State: " << m_Curr << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(InputBufferStatusChange)
		EVENT_CLASS_CATEGORY(EventCategoryInputBuffer)

	private:
		std::string m_Prev, m_Curr;
		unsigned int m_ID;
		size_t m_Width, m_DataWidth, m_DataHeight;
	};


}
