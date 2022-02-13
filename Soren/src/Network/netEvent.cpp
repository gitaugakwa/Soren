#include "olpch.h"
#include "Network/Network.h"

#include "Events/Event.h"
#include "Events/NetworkEvent.h"

namespace Soren {
namespace Neural {

	// OnEvent functions implementation

	void Network::AddEvent(const EventType& type, void(*func)(const Soren::Event&))
	{
		switch (type)
		{
		case EventType::NetworkInitialized:
		{
			m_Events.initialized.emplace_back(func);
			break;
		}
		case EventType::NetworkCreated:
		{
			m_Events.created.emplace_back(func);
			break;
		}
		case EventType::NetworkDeleted:
		{
			m_Events.deleted.emplace_back(func);
			break;
		}
		case EventType::NetworkReset:
		{
			m_Events.reset.emplace_back(func);
			break;
		}
		case EventType::NetworkCleared:
		{
			m_Events.cleared.emplace_back(func);
			break;
		}
		case EventType::NetworkDisabled:
		{
			m_Events.disabled.emplace_back(func);
			break;
		}
		case EventType::NetworkEnabled:
		{
			m_Events.enabled.emplace_back(func);
			break;
		}
		case EventType::NetworkCopied:
		{
			m_Events.copied.emplace_back(func);
			break;
		}
		case EventType::NetworkCopyAssigned:
		{
			m_Events.copyassign.emplace_back(func);
			break;
		}
		case EventType::NetworkMoved:
		{
			m_Events.moved.emplace_back(func);
			break;
		}
		case EventType::NetworkMoveAssigned:
		{
			m_Events.moveassign.emplace_back(func);
			break;
		}
		case EventType::NetworkActive:
		{
			m_Events.active.emplace_back(func);
			break;
		}
		case EventType::NetworkInactive:
		{
			m_Events.inactive.emplace_back(func);
			break;
		}
		case EventType::NetworkIterationStart:
		{
			m_Events.itstart.emplace_back(func);
			break;
		}
		case EventType::NetworkIterationStop:
		{
			m_Events.itstop.emplace_back(func);
			break;
		}
		case EventType::NetworkStatusChange:
		{
			m_Events.statuschange.emplace_back(func);
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid NetworkEvent type");
			assert(false);
		}
		}
		return;
	}

	void Network::DeleteEvent(const EventType& type, void(*func)(const Soren::Event&))
	{
		switch (type)
		{
		case EventType::NetworkInitialized:
		{
			auto pos = std::find(m_Events.initialized.begin(), m_Events.initialized.end(), func);
			if (pos != m_Events.initialized.end())
			{
				m_Events.initialized.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkCreated:
		{
			auto pos = std::find(m_Events.created.begin(), m_Events.created.end(), func);
			if (pos != m_Events.created.end())
			{
				m_Events.created.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkDeleted:
		{
			auto pos = std::find(m_Events.deleted.begin(), m_Events.deleted.end(), func);
			if (pos != m_Events.deleted.end())
			{
				m_Events.deleted.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkCleared:
		{
			auto pos = std::find(m_Events.cleared.begin(), m_Events.cleared.end(), func);
			if (pos != m_Events.cleared.end())
			{
				m_Events.cleared.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkReset:
		{
			auto pos = std::find(m_Events.reset.begin(), m_Events.reset.end(), func);
			if (pos != m_Events.reset.end())
			{
				m_Events.reset.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkDisabled:
		{
			auto pos = std::find(m_Events.disabled.begin(), m_Events.disabled.end(), func);
			if (pos != m_Events.disabled.end())
			{
				m_Events.disabled.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkEnabled:
		{
			auto pos = std::find(m_Events.enabled.begin(), m_Events.enabled.end(), func);
			if (pos != m_Events.enabled.end())
			{
				m_Events.enabled.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}

		case EventType::NetworkCopied:
		{
			auto pos = std::find(m_Events.copied.begin(), m_Events.copied.end(), func);
			if (pos != m_Events.copied.end())
			{
				m_Events.copied.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkCopyAssigned:
		{
			auto pos = std::find(m_Events.copyassign.begin(), m_Events.copyassign.end(), func);
			if (pos != m_Events.copyassign.end())
			{
				m_Events.copyassign.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkMoved:
		{
			auto pos = std::find(m_Events.moved.begin(), m_Events.moved.end(), func);
			if (pos != m_Events.moved.end())
			{
				m_Events.moved.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkMoveAssigned:
		{
			auto pos = std::find(m_Events.moveassign.begin(), m_Events.moveassign.end(), func);
			if (pos != m_Events.moveassign.end())
			{
				m_Events.moveassign.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkActive:
		{
			auto pos = std::find(m_Events.active.begin(), m_Events.active.end(), func);
			if (pos != m_Events.active.end())
			{
				m_Events.active.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkInactive:
		{
			auto pos = std::find(m_Events.active.begin(), m_Events.active.end(), func);
			if (pos != m_Events.active.end())
			{
				m_Events.active.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkIterationStart:
		{
			auto pos = std::find(m_Events.itstart.begin(), m_Events.itstart.end(), func);
			if (pos != m_Events.itstart.end())
			{
				m_Events.itstart.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkIterationStop:
		{
			auto pos = std::find(m_Events.itstop.begin(), m_Events.itstop.end(), func);
			if (pos != m_Events.itstop.end())
			{
				m_Events.itstop.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::NetworkStatusChange:
		{
			auto pos = std::find(m_Events.statuschange.begin(), m_Events.statuschange.end(), func);
			if (pos != m_Events.statuschange.end())
			{
				m_Events.statuschange.erase(pos);
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
			OL_CORE_ERROR("Enter Valid NetworkEvent type");
			assert(false);
		}
		}
	}

	void Network::DeleteEvent(const EventType& type, size_t number)
	{
		switch (type)
		{
		case EventType::NetworkInitialized:
		{
			auto pos = m_Events.initialized.begin() + number;
			if (pos < m_Events.initialized.end())
			{
				m_Events.initialized.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.initialized.size());
				assert(false);
			}
			break;
		}
		case EventType::NetworkCreated:
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
		case EventType::NetworkDeleted:
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
		case EventType::NetworkCleared:
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
		case EventType::NetworkReset:
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
		case EventType::NetworkDisabled:
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
		case EventType::NetworkEnabled:
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
		case EventType::NetworkCopied:
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
		case EventType::NetworkCopyAssigned:
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
		case EventType::NetworkMoved:
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
		case EventType::NetworkMoveAssigned:
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
		case EventType::NetworkActive:
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
		case EventType::NetworkInactive:
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
		case EventType::NetworkIterationStart:
		{
			auto pos = m_Events.itstart.begin() + number;
			if (pos < m_Events.itstart.end())
			{
				m_Events.itstart.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.itstart.size());
				assert(false);
			}
			break;
		}
		case EventType::NetworkIterationStop:
		{
			auto pos = m_Events.itstop.begin() + number;
			if (pos < m_Events.itstop.end())
			{
				m_Events.itstop.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.itstop.size());
				assert(false);
			}
			break;
		}
		case EventType::NetworkStatusChange:
		{
			auto pos = m_Events.statuschange.begin() + number;
			if (pos < m_Events.statuschange.end())
			{
				m_Events.statuschange.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, m_Events.statuschange.size());
				assert(false);
			}
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid NetworkEvent type");
			assert(false);
		}
		}
		return;
	}

	void Network::CheckAdditionalEvents(const Soren::Event& e)
	{
		switch (e.GetEventType())
		{
		case EventType::NetworkInitialized:
		{
			for (auto event : m_Events.initialized)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkCreated:
		{
			for (auto event : m_Events.created)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkDeleted:
		{
			for (auto event : m_Events.deleted)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkCleared:
		{
			for (auto event : m_Events.cleared)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkReset:
		{
			for (auto event : m_Events.reset)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkDisabled:
		{
			for (auto event : m_Events.disabled)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkEnabled:
		{
			for (auto event : m_Events.enabled)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkCopied:
		{
			for (auto event : m_Events.copied)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkCopyAssigned:
		{
			for (auto event : m_Events.copyassign)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkMoved:
		{
			for (auto event : m_Events.moved)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkMoveAssigned:
		{
			for (auto event : m_Events.moveassign)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkActive:
		{
			for (auto event : m_Events.active)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkInactive:
		{
			for (auto event : m_Events.inactive)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkIterationStart:
		{
			for (auto event : m_Events.itstart)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkIterationStop:
		{
			for (auto event : m_Events.itstop)
			{
				event(e);
			}
			break;
		}
		case EventType::NetworkStatusChange:
		{
			for (auto event : m_Events.statuschange)
			{
				event(e);
			}
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid NetworkEvent type");
			assert(false);
		}
		}
		return;
	}

	void Network::OnEvent(const Soren::Event& e)
	{
		//EventDispatcher dispatcher(e);

		//dispatcher.Dispatch<NetworkCreatedEvent>(BIND_EVENT_FN(Network::Init)); <- Might be virtuals
		//dispatcher.Dispatch<NetworkClosedEvent>(BIND_EVENT_FN(Network::Deinit)); <- Might be virtuals
		//That means that we can pass the raw eventtype to this functions for more complex calculations, not it cast to event


		switch (e.GetEventType())
		{
		case EventType::NetworkInitialized:
		{
			m_Callbacks.initialized(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkCreated:
		{
			m_Callbacks.created(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkDeleted:
		{
			m_Callbacks.deleted(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkCleared:
		{
			m_Callbacks.cleared(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkReset:
		{
			m_Callbacks.reset(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkDisabled:
		{
			m_Callbacks.disabled(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkEnabled:
		{
			m_Callbacks.enabled(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkCopied:
		{
			m_Callbacks.copied(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkCopyAssigned:
		{
			m_Callbacks.copyassign(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkMoved:
		{
			m_Callbacks.moved(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkMoveAssigned:
		{
			m_Callbacks.moveassign(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkActive:
		{
			m_Callbacks.active(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkInactive:
		{
			m_Callbacks.inactive(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkIterationStart:
		{
			m_Callbacks.itstart(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkIterationStop:
		{
			m_Callbacks.itstop(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::NetworkStatusChange:
		{
			m_Callbacks.statuschange(e);
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

		//dispatcher.Dispatch<NetworkClosedEvent>(BIND_EVENT_FN());
	}

	void Network::DeleteEventCallback(const EventType& type)
	{
		switch (type)
		{
		case EventType::NetworkCreated:
		{
			m_Callbacks.created = nullptr;
			break;
		}
		case EventType::NetworkDeleted:
		{
			m_Callbacks.deleted = nullptr;
			break;
		}
		case EventType::NetworkCleared:
		{
			m_Callbacks.cleared = nullptr;
			break;
		}
		case EventType::NetworkReset:
		{
			m_Callbacks.reset = nullptr;
			break;
		}
		case EventType::NetworkDisabled:
		{
			m_Callbacks.disabled = nullptr;
			break;
		}
		case EventType::NetworkEnabled:
		{
			m_Callbacks.enabled = nullptr;
			break;
		}
		case EventType::NetworkCopied:
		{
			m_Callbacks.copied = nullptr;
			break;
		}
		case EventType::NetworkCopyAssigned:
		{
			m_Callbacks.copyassign = nullptr;
			break;
		}
		case EventType::NetworkMoved:
		{
			m_Callbacks.moved = nullptr;
			break;
		}
		case EventType::NetworkMoveAssigned:
		{
			m_Callbacks.moveassign = nullptr;
			break;
		}
		case EventType::NetworkActive:
		{
			m_Callbacks.active = nullptr;
			break;
		}
		case EventType::NetworkInactive:
		{
			m_Callbacks.inactive = nullptr;
			break;
		}
		case EventType::NetworkIterationStart:
		{
			m_Callbacks.itstart = nullptr;
			break;
		}
		case EventType::NetworkIterationStop:
		{
			m_Callbacks.itstop = nullptr;
			break;
		}
		case EventType::NetworkStatusChange:
		{
			m_Callbacks.statuschange = nullptr;
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid NetworkEvent type");
			assert(false);
		}
		}
	}

	// Set callback function implementation

	void Network::SetNetworkEventfunc(const EventType& type, NetworkEventfunc func)
	{
		switch (type)
		{
		case EventType::NetworkCreated:
		{
			CHANGE_POINTERS(m_Callbacks.created, func);
			break;
		}
		case EventType::NetworkDeleted:
		{
			CHANGE_POINTERS(m_Callbacks.deleted, func);
			break;
		}
		case EventType::NetworkCleared:
		{
			CHANGE_POINTERS(m_Callbacks.cleared, func);
			break;
		}
		case EventType::NetworkReset:
		{
			CHANGE_POINTERS(m_Callbacks.reset, func);
			break;
		}
		case EventType::NetworkDisabled:
		{
			CHANGE_POINTERS(m_Callbacks.disabled, func);
			break;
		}
		case EventType::NetworkEnabled:
		{
			CHANGE_POINTERS(m_Callbacks.enabled, func);
			break;
		}
		case EventType::NetworkCopied:
		{
			CHANGE_POINTERS(m_Callbacks.copied, func);
			break;
		}
		case EventType::NetworkCopyAssigned:
		{
			CHANGE_POINTERS(m_Callbacks.copyassign, func);
			break;
		}
		case EventType::NetworkMoved:
		{
			CHANGE_POINTERS(m_Callbacks.moved, func);
			break;
		}
		case EventType::NetworkMoveAssigned:
		{
			CHANGE_POINTERS(m_Callbacks.moveassign, func);
			break;
		}
		case EventType::NetworkActive:
		{
			CHANGE_POINTERS(m_Callbacks.active, func);
			break;
		}
		case EventType::NetworkInactive:
		{
			CHANGE_POINTERS(m_Callbacks.inactive, func);
			break;
		}
		case EventType::NetworkIterationStart:
		{
			CHANGE_POINTERS(m_Callbacks.itstart, func);
			break;
		}
		case EventType::NetworkIterationStop:
		{
			CHANGE_POINTERS(m_Callbacks.itstop, func);
			break;
		}
		case EventType::NetworkStatusChange:
		{
			CHANGE_POINTERS(m_Callbacks.statuschange, func);
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid NetworkEvent type");
			assert(false);
		}
		}
		return;
	}

} // namespace Neural
} // namespace Soren