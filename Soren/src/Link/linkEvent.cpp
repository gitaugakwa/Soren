#include "olpch.h"
#include "Link/Link.h"

#include "Node/Node.h"
#include "Network/Network.h"

#include "Events/Event.h"
#include "Events/LinkEvent.h"

namespace Soren {
namespace Neural {

	// OnEvent function implementation

	void Link::AddEvent(const EventType& type, void(*func)(const Soren::Event&))
	{
		switch (type)
		{
		case EventType::LinkCreated:
		{
			network().m_LinkEvents.created.emplace_back(func);
			break;
		}
		case EventType::LinkDeleted:
		{
			network().m_LinkEvents.deleted.emplace_back(func);
			break;
		}
		case EventType::LinkReset:
		{
			network().m_LinkEvents.reset.emplace_back(func);
			break;
		}
		case EventType::LinkCleared:
		{
			network().m_LinkEvents.cleared.emplace_back(func);
			break;
		}
		case EventType::LinkDisabled:
		{
			network().m_LinkEvents.disabled.emplace_back(func);
			break;
		}
		case EventType::LinkEnabled:
		{
			network().m_LinkEvents.enabled.emplace_back(func);
			break;
		}
		case EventType::LinkCopied:
		{
			network().m_LinkEvents.copied.emplace_back(func);
			break;
		}
		case EventType::LinkCopyAssigned:
		{
			network().m_LinkEvents.copyassign.emplace_back(func);
			break;
		}
		case EventType::LinkMoved:
		{
			network().m_LinkEvents.moved.emplace_back(func);
			break;
		}
		case EventType::LinkMoveAssigned:
		{
			network().m_LinkEvents.moveassign.emplace_back(func);
			break;
		}
		case EventType::LinkActive:
		{
			network().m_LinkEvents.active.emplace_back(func);
			break;
		}
		case EventType::LinkInactive:
		{
			network().m_LinkEvents.inactive.emplace_back(func);
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid LinkEvent type");
			assert(false);
		}
		}
		return;
	}

	void Link::DeleteEvent(const EventType& type, void(*func)(const Soren::Event&))
	{
		switch (type)
		{
		case EventType::LinkCreated:
		{
			if (std::find(network().m_LinkEvents.created.begin(), network().m_LinkEvents.created.end(), func) != network().m_LinkEvents.created.end())
			{
				network().m_LinkEvents.created.erase(std::find(network().m_LinkEvents.created.begin(), network().m_LinkEvents.created.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::LinkDeleted:
		{
			if (std::find(network().m_LinkEvents.deleted.begin(), network().m_LinkEvents.deleted.end(), func) != network().m_LinkEvents.deleted.end())
			{
				network().m_LinkEvents.deleted.erase(std::find(network().m_LinkEvents.deleted.begin(), network().m_LinkEvents.deleted.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::LinkReset:
		{
			if (std::find(network().m_LinkEvents.reset.begin(), network().m_LinkEvents.reset.end(), func) != network().m_LinkEvents.reset.end())
			{
				network().m_LinkEvents.reset.erase(std::find(network().m_LinkEvents.reset.begin(), network().m_LinkEvents.reset.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::LinkCleared:
		{
			if (std::find(network().m_LinkEvents.cleared.begin(), network().m_LinkEvents.cleared.end(), func) != network().m_LinkEvents.cleared.end())
			{
				network().m_LinkEvents.cleared.erase(std::find(network().m_LinkEvents.cleared.begin(), network().m_LinkEvents.cleared.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::LinkDisabled:
		{
			if (std::find(network().m_LinkEvents.disabled.begin(), network().m_LinkEvents.disabled.end(), func) != network().m_LinkEvents.disabled.end())
			{
				network().m_LinkEvents.disabled.erase(std::find(network().m_LinkEvents.disabled.begin(), network().m_LinkEvents.disabled.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::LinkEnabled:
		{
			if (std::find(network().m_LinkEvents.enabled.begin(), network().m_LinkEvents.enabled.end(), func) != network().m_LinkEvents.enabled.end())
			{
				network().m_LinkEvents.enabled.erase(std::find(network().m_LinkEvents.enabled.begin(), network().m_LinkEvents.enabled.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::LinkCopied:
		{
			if (std::find(network().m_LinkEvents.copied.begin(), network().m_LinkEvents.copied.end(), func) != network().m_LinkEvents.copied.end())
			{
				network().m_LinkEvents.copied.erase(std::find(network().m_LinkEvents.copied.begin(), network().m_LinkEvents.copied.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::LinkCopyAssigned:
		{
			if (std::find(network().m_LinkEvents.copyassign.begin(), network().m_LinkEvents.copyassign.end(), func) != network().m_LinkEvents.copyassign.end())
			{
				network().m_LinkEvents.copyassign.erase(std::find(network().m_LinkEvents.copyassign.begin(), network().m_LinkEvents.copyassign.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::LinkMoved:
		{
			if (std::find(network().m_LinkEvents.moved.begin(), network().m_LinkEvents.moved.end(), func) != network().m_LinkEvents.moved.end())
			{
				network().m_LinkEvents.moved.erase(std::find(network().m_LinkEvents.moved.begin(), network().m_LinkEvents.moved.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::LinkMoveAssigned:
		{
			if (std::find(network().m_LinkEvents.moveassign.begin(), network().m_LinkEvents.moveassign.end(), func) != network().m_LinkEvents.moveassign.end())
			{
				network().m_LinkEvents.moveassign.erase(std::find(network().m_LinkEvents.moveassign.begin(), network().m_LinkEvents.moveassign.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::LinkActive:
		{
			if (std::find(network().m_LinkEvents.active.begin(), network().m_LinkEvents.active.end(), func) != network().m_LinkEvents.active.end())
			{
				network().m_LinkEvents.active.erase(std::find(network().m_LinkEvents.active.begin(), network().m_LinkEvents.active.end(), func));
			}
			else
			{
				OL_CORE_ERROR("Function not found");
				assert(false);
			}
			break;
		}
		case EventType::LinkInactive:
		{
			if (std::find(network().m_LinkEvents.inactive.begin(), network().m_LinkEvents.inactive.end(), func) != network().m_LinkEvents.inactive.end())
			{
				network().m_LinkEvents.inactive.erase(std::find(network().m_LinkEvents.inactive.begin(), network().m_LinkEvents.inactive.end(), func));
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
			OL_CORE_ERROR("Enter Valid LinkEvent type");
			assert(false);
		}
		}
	}

	void Link::DeleteEvent(const EventType& type, const unsigned int& number)
	{
		switch (type)
		{
		case EventType::LinkCreated:
		{
			auto pos = network().m_LinkEvents.created.begin() + number;
			if (pos < network().m_LinkEvents.created.end())
			{
				network().m_LinkEvents.created.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, network().m_LinkEvents.created.size());
				assert(false);
			}
			break;
		}
		case EventType::LinkDeleted:
		{
			auto pos = network().m_LinkEvents.deleted.begin() + number;
			if (pos < network().m_LinkEvents.deleted.end())
			{
				network().m_LinkEvents.deleted.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, network().m_LinkEvents.deleted.size());
				assert(false);
			}
			break;
		}
		case EventType::LinkReset:
		{
			auto pos = network().m_LinkEvents.reset.begin() + number;
			if (pos < network().m_LinkEvents.reset.end())
			{
				network().m_LinkEvents.reset.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, network().m_LinkEvents.reset.size());
				assert(false);
			}
			break;
		}
		case EventType::LinkCleared:
		{
			auto pos = network().m_LinkEvents.cleared.begin() + number;
			if (pos < network().m_LinkEvents.cleared.end())
			{
				network().m_LinkEvents.cleared.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, network().m_LinkEvents.cleared.size());
				assert(false);
			}
			break;
		}
		case EventType::LinkDisabled:
		{
			auto pos = network().m_LinkEvents.disabled.begin() + number;
			if (pos < network().m_LinkEvents.disabled.end())
			{
				network().m_LinkEvents.disabled.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, network().m_LinkEvents.disabled.size());
				assert(false);
			}
			break;
		}
		case EventType::LinkEnabled:
		{
			auto pos = network().m_LinkEvents.enabled.begin() + number;
			if (pos < network().m_LinkEvents.enabled.end())
			{
				network().m_LinkEvents.enabled.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, network().m_LinkEvents.enabled.size());
				assert(false);
			}
			break;
		}
		case EventType::LinkCopied:
		{
			auto pos = network().m_LinkEvents.copied.begin() + number;
			if (pos < network().m_LinkEvents.copied.end())
			{
				network().m_LinkEvents.copied.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, network().m_LinkEvents.copied.size());
				assert(false);
			}
			break;
		}
		case EventType::LinkCopyAssigned:
		{
			auto pos = network().m_LinkEvents.copyassign.begin() + number;
			if (pos < network().m_LinkEvents.copyassign.end())
			{
				network().m_LinkEvents.copyassign.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, network().m_LinkEvents.copyassign.size());
				assert(false);
			}
			break;
		}
		case EventType::LinkMoved:
		{
			auto pos = network().m_LinkEvents.moved.begin() + number;
			if (pos < network().m_LinkEvents.moved.end())
			{
				network().m_LinkEvents.moved.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, network().m_LinkEvents.moved.size());
				assert(false);
			}
			break;
		}
		case EventType::LinkMoveAssigned:
		{
			auto pos = network().m_LinkEvents.moveassign.begin() + number;
			if (pos < network().m_LinkEvents.moveassign.end())
			{
				network().m_LinkEvents.moveassign.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, network().m_LinkEvents.moveassign.size());
				assert(false);
			}
			break;
		}
		case EventType::LinkActive:
		{
			auto pos = network().m_LinkEvents.active.begin() + number;
			if (pos < network().m_LinkEvents.active.end())
			{
				network().m_LinkEvents.active.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, network().m_LinkEvents.active.size());
				assert(false);
			}
			break;
		}
		case EventType::LinkInactive:
		{
			auto pos = network().m_LinkEvents.inactive.begin() + number;
			if (pos < network().m_LinkEvents.inactive.end())
			{
				network().m_LinkEvents.inactive.erase(pos);
			}
			else
			{
				OL_CORE_ERROR("Number{{0}} is greater than the current number of functions{{1}}", number, network().m_LinkEvents.inactive.size());
				assert(false);
			}
			break;
		}

		default:
		{
			OL_CORE_ERROR("Enter Valid LinkEvent type");
			assert(false);
		}
		}
		return;
	}

	void Link::CheckAdditionalEvents(const Soren::Event& e)
	{
		switch (e.GetEventType())
		{
		case EventType::LinkCreated:
		{
			for (auto event : network().m_LinkEvents.created)
			{
				event(e);
			}
			break;
		}
		case EventType::LinkDeleted:
		{
			for (auto event : network().m_LinkEvents.deleted)
			{
				event(e);
			}
			break;
		}
		case EventType::LinkReset:
		{
			for (auto event : network().m_LinkEvents.reset)
			{
				event(e);
			}
			break;
		}
		case EventType::LinkCleared:
		{
			for (auto event : network().m_LinkEvents.cleared)
			{
				event(e);
			}
			break;
		}
		case EventType::LinkDisabled:
		{
			for (auto event : network().m_LinkEvents.disabled)
			{
				event(e);
			}
			break;
		}
		case EventType::LinkEnabled:
		{
			for (auto event : network().m_LinkEvents.enabled)
			{
				event(e);
			}
			break;
		}
		case EventType::LinkCopied:
		{
			for (auto event : network().m_LinkEvents.copied)
			{
				event(e);
			}
			break;
		}
		case EventType::LinkCopyAssigned:
		{
			for (auto event : network().m_LinkEvents.copyassign)
			{
				event(e);
			}
			break;
		}
		case EventType::LinkMoved:
		{
			for (auto event : network().m_LinkEvents.moved)
			{
				event(e);
			}
			break;
		}
		case EventType::LinkMoveAssigned:
		{
			for (auto event : network().m_LinkEvents.moveassign)
			{
				event(e);
			}
			break;
		}
		case EventType::LinkActive:
		{
			for (auto event : network().m_LinkEvents.active)
			{
				event(e);
			}
			break;
		}
		case EventType::LinkInactive:
		{
			for (auto event : network().m_LinkEvents.inactive)
			{
				event(e);
			}
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid LinkEvent type");
			assert(false);
		}
		}
		return;
	}

	void Link::OnEvent(const Soren::Event& e)
	{
		if (!m_Running) return;
		//EventDispatcher dispatcher(e);

		//dispatcher.Dispatch<NetworkCreatedEvent>(BIND_EVENT_FN(Link::Init)); <- Might be virtuals
		//dispatcher.Dispatch<NetworkClosedEvent>(BIND_EVENT_FN(Link::Deinit)); <- Might be virtuals
		//That means that we can pass the raw eventtype to this functions for more complex calculations, not it cast to event

		switch (e.GetEventType())
		{
		case EventType::LinkCreated:
		{
			network().m_LinkCallbacks.created(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LinkDeleted:
		{
			network().m_LinkCallbacks.deleted(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LinkReset:
		{
			network().m_LinkCallbacks.reset(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LinkCleared:
		{
			network().m_LinkCallbacks.cleared(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LinkDisabled:
		{
			network().m_LinkCallbacks.disabled(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LinkEnabled:
		{
			network().m_LinkCallbacks.enabled(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LinkCopied:
		{
			network().m_LinkCallbacks.copied(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LinkCopyAssigned:
		{
			network().m_LinkCallbacks.copyassign(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LinkMoved:
		{
			network().m_LinkCallbacks.moved(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LinkMoveAssigned:
		{
			network().m_LinkCallbacks.moveassign(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LinkActive:
		{
			network().m_LinkCallbacks.active(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LinkInactive:
		{
			network().m_LinkCallbacks.inactive(e);
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

	void Link::SetEventCallbackFunction(const EventCallbackFn& callback)
	{
		network().m_LinkEventCallback = callback;
	}
	void Link::DefaultLinkCallbacks()
	{
		network().m_LinkCallbacks.Default();
	}
	void Link::DeleteEventCallback(const EventType& type)
	{
		switch (type)
		{
		case EventType::LinkCreated:
		{
			network().m_LinkCallbacks.created = nullptr;
			break;
		}
		case EventType::LinkDeleted:
		{
			network().m_LinkCallbacks.deleted = nullptr;
			break;
		}
		case EventType::LinkReset:
		{
			network().m_LinkCallbacks.reset = nullptr;
			break;
		}
		case EventType::LinkCleared:
		{
			network().m_LinkCallbacks.cleared = nullptr;
			break;
		}
		case EventType::LinkDisabled:
		{
			network().m_LinkCallbacks.disabled = nullptr;
			break;
		}
		case EventType::LinkEnabled:
		{
			network().m_LinkCallbacks.enabled = nullptr;
			break;
		}
		case EventType::LinkCopied:
		{
			network().m_LinkCallbacks.copied = nullptr;
			break;
		}
		case EventType::LinkCopyAssigned:
		{
			network().m_LinkCallbacks.copyassign = nullptr;
			break;
		}
		case EventType::LinkMoved:
		{
			network().m_LinkCallbacks.moved = nullptr;
			break;
		}
		case EventType::LinkMoveAssigned:
		{
			network().m_LinkCallbacks.moveassign = nullptr;
			break;
		}
		case EventType::LinkActive:
		{
			network().m_LinkCallbacks.active = nullptr;
			break;
		}
		case EventType::LinkInactive:
		{
			network().m_LinkCallbacks.inactive = nullptr;
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid LinkEvent type");
			assert(false);
		}
		}
	}

	// Set callback functions implementation

	void Link::SetLinkEventfunc(const EventType& type, LinkEventfunc func)
	{
		switch (type)
		{
		case EventType::LinkCreated:
		{
			CHANGE_POINTERS(network().m_LinkCallbacks.created, func);
			break;
		}
		case EventType::LinkDeleted:
		{
			CHANGE_POINTERS(network().m_LinkCallbacks.deleted, func);
			break;
		}
		case EventType::LinkReset:
		{
			CHANGE_POINTERS(network().m_LinkCallbacks.reset, func);
			break;
		}
		case EventType::LinkCleared:
		{
			CHANGE_POINTERS(network().m_LinkCallbacks.cleared, func);
			break;
		}
		case EventType::LinkDisabled:
		{
			CHANGE_POINTERS(network().m_LinkCallbacks.disabled, func);
			break;
		}
		case EventType::LinkEnabled:
		{
			CHANGE_POINTERS(network().m_LinkCallbacks.enabled, func);
			break;
		}
		case EventType::LinkCopied:
		{
			CHANGE_POINTERS(network().m_LinkCallbacks.copied, func);
			break;
		}
		case EventType::LinkCopyAssigned:
		{
			CHANGE_POINTERS(network().m_LinkCallbacks.copyassign, func);
			break;
		}
		case EventType::LinkMoved:
		{
			CHANGE_POINTERS(network().m_LinkCallbacks.moved, func);
			break;
		}
		case EventType::LinkMoveAssigned:
		{
			CHANGE_POINTERS(network().m_LinkCallbacks.moveassign, func);
			break;
		}
		case EventType::LinkActive:
		{
			CHANGE_POINTERS(network().m_LinkCallbacks.active, func);
			break;
		}
		case EventType::LinkInactive:
		{
			CHANGE_POINTERS(network().m_LinkCallbacks.inactive, func);
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid LinkEvent type");
			assert(false);
		}
		}
		return;
	}

} // namespace Neural
} // namespace Soren