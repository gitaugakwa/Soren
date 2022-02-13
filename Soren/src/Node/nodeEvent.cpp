#include "olpch.h"
#include "Node/Node.h"

#include "Layer/Layer.h"
#include "Network/Network.h"

#include "Events/Event.h"
#include "Events/NodeEvent.h"

namespace Soren {
namespace Neural {

	// OnEvent function implementation

	void Node::AddEvent(const EventType& type, void(*func)(const Soren::Event&))
	{
		switch (type)
		{
		case EventType::NodeCreated:
		{
			m_Events.created.emplace_back(func);
			break;
		}
		case EventType::NodeDeleted:
		{
			m_Events.deleted.emplace_back(func);
			break;
		}
		case EventType::NodeReset:
		{
			m_Events.reset.emplace_back(func);
			break;
		}
		case EventType::NodeCleared:
		{
			m_Events.cleared.emplace_back(func);
			break;
		}
		case EventType::NodeDisabled:
		{
			m_Events.disabled.emplace_back(func);
			break;
		}
		case EventType::NodeEnabled:
		{
			m_Events.enabled.emplace_back(func);
			break;
		}
		case EventType::NodeCopied:
		{
			m_Events.copied.emplace_back(func);
			break;
		}
		case EventType::NodeCopyAssigned:
		{
			m_Events.copyassign.emplace_back(func);
			break;
		}
		case EventType::NodeMoved:
		{
			m_Events.moved.emplace_back(func);
			break;
		}
		case EventType::NodeMoveAssigned:
		{
			m_Events.moveassign.emplace_back(func);
			break;
		}
		case EventType::NodeActive:
		{
			m_Events.active.emplace_back(func);
			break;
		}
		case EventType::NodeInactive:
		{
			m_Events.inactive.emplace_back(func);
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid NodeEvent type");
			assert(false);
		}
		}
		return;
	}

	void Node::DeleteEvent(const EventType& type, void(*func)(const Soren::Event&))
	{
		switch (type)
		{
		case EventType::NodeCreated:
		{
			if (std::find(m_Events.created.begin(), m_Events.created.end(), func) != m_Events.created.end())
			{
				m_Events.created.erase(std::find(m_Events.created.begin(), m_Events.created.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NodeDeleted:
		{
			if (std::find(m_Events.deleted.begin(), m_Events.deleted.end(), func) != m_Events.deleted.end())
			{
				m_Events.deleted.erase(std::find(m_Events.deleted.begin(), m_Events.deleted.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NodeReset:
		{
			if (std::find(m_Events.reset.begin(), m_Events.reset.end(), func) != m_Events.reset.end())
			{
				m_Events.reset.erase(std::find(m_Events.reset.begin(), m_Events.reset.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NodeCleared:
		{
			if (std::find(m_Events.cleared.begin(), m_Events.cleared.end(), func) != m_Events.cleared.end())
			{
				m_Events.cleared.erase(std::find(m_Events.cleared.begin(), m_Events.cleared.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NodeDisabled:
		{
			if (std::find(m_Events.disabled.begin(), m_Events.disabled.end(), func) != m_Events.disabled.end())
			{
				m_Events.disabled.erase(std::find(m_Events.disabled.begin(), m_Events.disabled.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NodeEnabled:
		{
			if (std::find(m_Events.enabled.begin(), m_Events.enabled.end(), func) != m_Events.enabled.end())
			{
				m_Events.enabled.erase(std::find(m_Events.enabled.begin(), m_Events.enabled.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NodeCopied:
		{
			if (std::find(m_Events.copied.begin(), m_Events.copied.end(), func) != m_Events.copied.end())
			{
				m_Events.copied.erase(std::find(m_Events.copied.begin(), m_Events.copied.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NodeCopyAssigned:
		{
			if (std::find(m_Events.copyassign.begin(), m_Events.copyassign.end(), func) != m_Events.copyassign.end())
			{
				m_Events.copyassign.erase(std::find(m_Events.copyassign.begin(), m_Events.copyassign.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NodeMoved:
		{
			if (std::find(m_Events.moved.begin(), m_Events.moved.end(), func) != m_Events.moved.end())
			{
				m_Events.moved.erase(std::find(m_Events.moved.begin(), m_Events.moved.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NodeMoveAssigned:
		{
			if (std::find(m_Events.moveassign.begin(), m_Events.moveassign.end(), func) != m_Events.moveassign.end())
			{
				m_Events.moveassign.erase(std::find(m_Events.moveassign.begin(), m_Events.moveassign.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NodeActive:
		{
			if (std::find(m_Events.active.begin(), m_Events.active.end(), func) != m_Events.active.end())
			{
				m_Events.active.erase(std::find(m_Events.active.begin(), m_Events.active.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NodeInactive:
		{
			if (std::find(m_Events.inactive.begin(), m_Events.inactive.end(), func) != m_Events.inactive.end())
			{
				m_Events.inactive.erase(std::find(m_Events.inactive.begin(), m_Events.inactive.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid NodeEvent type");
			assert(false);
		}
		}
	}

	void Node::DeleteEvent(const EventType& type, const unsigned int& number)
	{
		switch (type)
		{
		case EventType::NodeCreated:
		{
			auto pos = m_Events.created.begin() + number;
			if (pos < m_Events.created.end())
			{
				m_Events.created.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.created.size());
				assert(false);
			}
			break;
		}
		case EventType::NodeDeleted:
		{
			auto pos = m_Events.deleted.begin() + number;
			if (pos < m_Events.deleted.end())
			{
				m_Events.deleted.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.deleted.size());
				assert(false);
			}
			break;
		}
		case EventType::NodeReset:
		{
			auto pos = m_Events.reset.begin() + number;
			if (pos < m_Events.reset.end())
			{
				m_Events.reset.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.reset.size());
				assert(false);
			}
			break;
		}
		case EventType::NodeCleared:
		{
			auto pos = m_Events.cleared.begin() + number;
			if (pos < m_Events.cleared.end())
			{
				m_Events.cleared.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.cleared.size());
				assert(false);
			}
			break;
		}
		case EventType::NodeDisabled:
		{
			auto pos = m_Events.disabled.begin() + number;
			if (pos < m_Events.disabled.end())
			{
				m_Events.disabled.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.disabled.size());
				assert(false);
			}
			break;
		}
		case EventType::NodeEnabled:
		{
			auto pos = m_Events.enabled.begin() + number;
			if (pos < m_Events.enabled.end())
			{
				m_Events.enabled.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.enabled.size());
				assert(false);
			}
			break;
		}
		case EventType::NodeCopied:
		{
			auto pos = m_Events.copied.begin() + number;
			if (pos < m_Events.copied.end())
			{
				m_Events.copied.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.copied.size());
				assert(false);
			}
			break;
		}
		case EventType::NodeCopyAssigned:
		{
			auto pos = m_Events.copyassign.begin() + number;
			if (pos < m_Events.copyassign.end())
			{
				m_Events.copyassign.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.copyassign.size());
				assert(false);
			}
			break;
		}
		case EventType::NodeMoved:
		{
			auto pos = m_Events.moved.begin() + number;
			if (pos < m_Events.moved.end())
			{
				m_Events.moved.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.moved.size());
				assert(false);
			}
			break;
		}
		case EventType::NodeMoveAssigned:
		{
			auto pos = m_Events.moveassign.begin() + number;
			if (pos < m_Events.moveassign.end())
			{
				m_Events.moveassign.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.moveassign.size());
				assert(false);
			}
			break;
		}
		case EventType::NodeActive:
		{
			auto pos = m_Events.active.begin() + number;
			if (pos < m_Events.active.end())
			{
				m_Events.active.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.active.size());
				assert(false);
			}
			break;
		}
		case EventType::NodeInactive:
		{
			auto pos = m_Events.inactive.begin() + number;
			if (pos < m_Events.inactive.end())
			{
				m_Events.inactive.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.inactive.size());
				assert(false);
			}
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid NodeEvent type");
			assert(false);
		}
		}
		return;
	}

	void Node::CheckAdditionalEvents(const Soren::Event& e)
	{
		switch (e.GetEventType())
		{
		case EventType::NodeCreated:
		{
			for (auto event : m_Events.created)
			{
				event(e);
			}
			break;
		}
		case EventType::NodeDeleted:
		{
			for (auto event : m_Events.deleted)
			{
				event(e);
			}
			break;
		}
		case EventType::NodeReset:
		{
			for (auto event : m_Events.reset)
			{
				event(e);
			}
			break;
		}
		case EventType::NodeCleared:
		{
			for (auto event : m_Events.cleared)
			{
				event(e);
			}
			break;
		}
		case EventType::NodeDisabled:
		{
			for (auto event : m_Events.disabled)
			{
				event(e);
			}
			break;
		}
		case EventType::NodeEnabled:
		{
			for (auto event : m_Events.enabled)
			{
				event(e);
			}
			break;
		}
		case EventType::NodeCopied:
		{
			for (auto event : m_Events.copied)
			{
				event(e);
			}
			break;
		}
		case EventType::NodeCopyAssigned:
		{
			for (auto event : m_Events.copyassign)
			{
				event(e);
			}
			break;
		}
		case EventType::NodeMoved:
		{
			for (auto event : m_Events.moved)
			{
				event(e);
			}
			break;
		}
		case EventType::NodeMoveAssigned:
		{
			for (auto event : m_Events.moveassign)
			{
				event(e);
			}
			break;
		}
		case EventType::NodeActive:
		{
			for (auto event : m_Events.active)
			{
				event(e);
			}
			break;
		}
		case EventType::NodeInactive:
		{
			for (auto event : m_Events.inactive)
			{
				event(e);
			}
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid NodeEvent type");
			assert(false);
		}
		}
		return;

	}

	void Node::OnEvent(const Soren::Event& e)
	{
		if (!m_Running) return;
		//EventDispatcher dispatcher(e);

		//dispatcher.Dispatch<NetworkCreatedEvent>(BIND_EVENT_FN(Node::Init)); <- Might be virtuals
		//dispatcher.Dispatch<NetworkClosedEvent>(BIND_EVENT_FN(Node::Deinit)); <- Might be virtuals
		//That means that we can pass the raw eventtype to this functions for more complex calculations, not it cast to event


		switch (e.GetEventType())
		{
		case EventType::NodeCreated:
		{
			m_Callbacks.created(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NodeDeleted:
		{
			m_Callbacks.deleted(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NodeReset:
		{
			m_Callbacks.reset(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NodeCleared:
		{
			m_Callbacks.cleared(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NodeDisabled:
		{
			m_Callbacks.disabled(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NodeEnabled:
		{
			m_Callbacks.enabled(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NodeCopied:
		{
			m_Callbacks.copied(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NodeCopyAssigned:
		{
			m_Callbacks.copyassign(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NodeMoved:
		{
			m_Callbacks.moved(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NodeMoveAssigned:
		{
			m_Callbacks.moveassign(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NodeActive:
		{
			m_Callbacks.active(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NodeInactive:
		{
			m_Callbacks.inactive(e);
			CheckAdditionalEvents(e);
			break;
		}
		default:
		{
			OL_CORE_TRACE(e);
			CheckAdditionalEvents(e);
		}
		}
		return;

		//dispatcher.Dispatch<NodeClosedEvent>(BIND_EVENT_FN());
	}

	void Node::SetEventCallbackFunction(const EventCallbackFn& callback)
	{
		network().m_NodeEventCallback = callback;
	}
	void Node::DeleteEventCallback(const EventType& type)
	{
		switch (type)
		{
		case EventType::NodeCreated:
		{
			m_Callbacks.created = nullptr;
			break;
		}
		case EventType::NodeDeleted:
		{
			m_Callbacks.deleted = nullptr;
			break;
		}
		case EventType::NodeReset:
		{
			m_Callbacks.reset = nullptr;
			break;
		}
		case EventType::NodeCleared:
		{
			m_Callbacks.cleared = nullptr;
			break;
		}
		case EventType::NodeDisabled:
		{
			m_Callbacks.disabled = nullptr;
			break;
		}
		case EventType::NodeEnabled:
		{
			m_Callbacks.enabled = nullptr;
			break;
		}
		case EventType::NodeCopied:
		{
			m_Callbacks.copied = nullptr;
			break;
		}
		case EventType::NodeCopyAssigned:
		{
			m_Callbacks.copyassign = nullptr;
			break;
		}
		case EventType::NodeMoved:
		{
			m_Callbacks.moved = nullptr;
			break;
		}
		case EventType::NodeMoveAssigned:
		{
			m_Callbacks.moveassign = nullptr;
			break;
		}
		case EventType::NodeActive:
		{
			m_Callbacks.active = nullptr;
			break;
		}
		case EventType::NodeInactive:
		{
			m_Callbacks.inactive = nullptr;
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid NodeEvent type");
			assert(false);
		}
		}
	}

	// Set callback functions implementation

	void Node::SetNodeEventfunc(const EventType& type, NodeEventfunc func)
	{
		switch (type)
		{
		case EventType::NodeCreated:
		{
			CHANGE_POINTERS(m_Callbacks.created, func);
			break;
		}
		case EventType::NodeDeleted:
		{
			CHANGE_POINTERS(m_Callbacks.deleted, func);
			break;
		}
		case EventType::NodeReset:
		{
			CHANGE_POINTERS(m_Callbacks.reset, func);
			break;
		}
		case EventType::NodeCleared:
		{
			CHANGE_POINTERS(m_Callbacks.cleared, func);
			break;
		}
		case EventType::NodeDisabled:
		{
			CHANGE_POINTERS(m_Callbacks.disabled, func);
			break;
		}
		case EventType::NodeEnabled:
		{
			CHANGE_POINTERS(m_Callbacks.enabled, func);
			break;
		}
		case EventType::NodeCopied:
		{
			CHANGE_POINTERS(m_Callbacks.copied, func);
			break;
		}
		case EventType::NodeCopyAssigned:
		{
			CHANGE_POINTERS(m_Callbacks.copyassign, func);
			break;
		}
		case EventType::NodeMoved:
		{
			CHANGE_POINTERS(m_Callbacks.moved, func);
			break;
		}
		case EventType::NodeMoveAssigned:
		{
			CHANGE_POINTERS(m_Callbacks.moveassign, func);
			break;
		}
		case EventType::NodeActive:
		{
			CHANGE_POINTERS(m_Callbacks.active, func);
			break;
		}
		case EventType::NodeInactive:
		{
			CHANGE_POINTERS(m_Callbacks.inactive, func);
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid NodeEvent type");
			assert(false);
		}
		}
		return;
	}
} // namespace Neural
} // namespace Soren