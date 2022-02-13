#include "olpch.h"
#include "Layer/Layer.h"

#include "Network/Network.h"

#include "Events/Event.h"
#include "Events/LayerEvent.h"

namespace Soren {
namespace Neural {

	// OnEvent function implementation

	void Layer::AddEvent(const EventType& type, void(*func)(const Soren::Event&))
	{
		switch (type)
		{
		case EventType::LayerCreated:
		{
			m_Events.created.emplace_back(func);
			break;
		}
		case EventType::LayerDeleted:
		{
			m_Events.deleted.emplace_back(func);
			break;
		}
		case EventType::LayerReset:
		{
			m_Events.reset.emplace_back(func);
			break;
		}
		case EventType::LayerDisabled:
		{
			m_Events.disabled.emplace_back(func);
			break;
		}
		case EventType::LayerEnabled:
		{
			m_Events.enabled.emplace_back(func);
			break;
		}
		case EventType::LayerCleared:
		{
			m_Events.cleared.emplace_back(func);
			break;
		}
		case EventType::LayerCopied:
		{
			m_Events.copied.emplace_back(func);
			break;
		}
		case EventType::LayerCopyAssigned:
		{
			m_Events.copyassign.emplace_back(func);
			break;
		}
		case EventType::LayerMoved:
		{
			m_Events.moved.emplace_back(func);
			break;
		}
		case EventType::LayerMoveAssigned:
		{
			m_Events.moveassign.emplace_back(func);
			break;
		}
		case EventType::LayerActive:
		{
			m_Events.active.emplace_back(func);
			break;
		}
		case EventType::LayerInactive:
		{
			m_Events.inactive.emplace_back(func);
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid LayerEvent type");
			assert(false);
		}
		}
		return;
	}

	void Layer::DeleteEvent(const EventType& type, void(*func)(const Soren::Event&))
	{
		switch (type)
		{
		case EventType::LayerCreated:
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
		case EventType::LayerDeleted:
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
		case EventType::LayerReset:
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
		case EventType::LayerDisabled:
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
		case EventType::LayerEnabled:
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
		case EventType::LayerCleared:
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
		case EventType::LayerCopied:
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
		case EventType::LayerCopyAssigned:
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
		case EventType::LayerMoved:
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
		case EventType::LayerMoveAssigned:
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
		case EventType::LayerActive:
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
		case EventType::LayerInactive:
		{
			auto pos = std::find(m_Events.inactive.begin(), m_Events.inactive.end(), func);
			if (pos != m_Events.inactive.end())
			{
				m_Events.inactive.erase(pos);
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
			OL_CORE_ERROR("Enter Valid LayerEvent type");
			assert(false);
		}
		}
	}

	void Layer::DeleteEvent(const EventType& type, size_t number)
	{
		switch (type)
		{
		case EventType::LayerCreated:
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
		case EventType::LayerDeleted:
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
		case EventType::LayerReset:
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
		case EventType::LayerDisabled:
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
		case EventType::LayerEnabled:
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
		case EventType::LayerCleared:
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
		case EventType::LayerCopied:
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
		case EventType::LayerCopyAssigned:
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
		case EventType::LayerMoved:
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
		case EventType::LayerMoveAssigned:
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
		case EventType::LayerActive:
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
		case EventType::LayerInactive:
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
			OL_CORE_ERROR("Enter Valid LayerEvent type");
			assert(false);
		}
		}
		return;
	}

	void Layer::CheckAdditionalEvents(const Soren::Event& e)
	{
		switch (e.GetEventType())
		{
		case EventType::LayerCreated:
		{
			for (auto event : m_Events.created)
			{
				event(e);
			}
			break;
		}
		case EventType::LayerDeleted:
		{
			for (auto event : m_Events.deleted)
			{
				event(e);
			}
			break;
		}
		case EventType::LayerReset:
		{
			for (auto event : m_Events.reset)
			{
				event(e);
			}
			break;
		}
		case EventType::LayerDisabled:
		{
			for (auto event : m_Events.disabled)
			{
				event(e);
			}
			break;
		}
		case EventType::LayerEnabled:
		{
			for (auto event : m_Events.enabled)
			{
				event(e);
			}
			break;
		}
		case EventType::LayerCleared:
		{
			for (auto event : m_Events.cleared)
			{
				event(e);
			}
			break;
		}
		case EventType::LayerCopied:
		{
			for (auto event : m_Events.copied)
			{
				event(e);
			}
			break;
		}
		case EventType::LayerCopyAssigned:
		{
			for (auto event : m_Events.copyassign)
			{
				event(e);
			}
			break;
		}
		case EventType::LayerMoved:
		{
			for (auto event : m_Events.moved)
			{
				event(e);
			}
			break;
		}
		case EventType::LayerMoveAssigned:
		{
			for (auto event : m_Events.moveassign)
			{
				event(e);
			}
			break;
		}
		case EventType::LayerActive:
		{
			for (auto event : m_Events.active)
			{
				event(e);
			}
			break;
		}
		case EventType::LayerInactive:
		{
			for (auto event : m_Events.inactive)
			{
				event(e);
			}
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid LayerEvent type");
			assert(false);
		}
		}
		return;

	}

	void Layer::OnEvent(const Soren::Event& e)
	{
		//EventDispatcher dispatcher(e);

		//dispatcher.Dispatch<NetworkCreatedEvent>(BIND_EVENT_FN(Layer::Init)); <- Might be virtuals
		//dispatcher.Dispatch<NetworkClosedEvent>(BIND_EVENT_FN(Layer::Deinit)); <- Might be virtuals
		//That means that we can pass the raw eventtype to this functions for more complex calculations, not it cast to event


		switch (e.GetEventType())
		{
		case EventType::LayerCreated:
		{
			m_Callbacks.created(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LayerDeleted:
		{
			m_Callbacks.deleted(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LayerReset:
		{
			m_Callbacks.reset(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LayerDisabled:
		{
			m_Callbacks.disabled(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LayerEnabled:
		{
			m_Callbacks.enabled(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LayerCleared:
		{
			m_Callbacks.cleared(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LayerCopied:
		{
			m_Callbacks.copied(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LayerCopyAssigned:
		{
			m_Callbacks.copyassign(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LayerMoved:
		{
			m_Callbacks.moved(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LayerMoveAssigned:
		{
			m_Callbacks.moveassign(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LayerActive:
		{
			m_Callbacks.active(e);
			CheckAdditionalEvents(e);
			break;
		}
		case EventType::LayerInactive:
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

		//dispatcher.Dispatch<NetworkClosedEvent>(BIND_EVENT_FN());
	}

	void Layer::SetEventCallbackFunction(const EventCallbackFn& callback)
	{
		m_Network.m_LayerEventCallback = callback;
	}
	void Layer::DeleteEventCallback(const EventType& type)
	{
		switch (type)
		{
		case EventType::LayerCreated:
		{
			m_Callbacks.created = nullptr;
			break;
		}
		case EventType::LayerDeleted:
		{
			m_Callbacks.deleted = nullptr;
			break;
		}
		case EventType::LayerReset:
		{
			m_Callbacks.reset = nullptr;
			break;
		}
		case EventType::LayerDisabled:
		{
			m_Callbacks.disabled = nullptr;
			break;
		}
		case EventType::LayerEnabled:
		{
			m_Callbacks.enabled = nullptr;
			break;
		}
		case EventType::LayerCleared:
		{
			m_Callbacks.cleared = nullptr;
			break;
		}
		case EventType::LayerCopied:
		{
			m_Callbacks.copied = nullptr;
			break;
		}
		case EventType::LayerCopyAssigned:
		{
			m_Callbacks.copyassign = nullptr;
			break;
		}
		case EventType::LayerMoved:
		{
			m_Callbacks.moved = nullptr;
			break;
		}
		case EventType::LayerMoveAssigned:
		{
			m_Callbacks.moveassign = nullptr;
			break;
		}
		case EventType::LayerActive:
		{
			m_Callbacks.active = nullptr;
			break;
		}
		case EventType::LayerInactive:
		{
			m_Callbacks.inactive = nullptr;
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid LayerEvent type");
			assert(false);
		}
		}
	}

	// Set callback functions implementation

	void Layer::SetLayerEventfunc(const EventType& type, LayerEventfunc func)
	{
		switch (type)
		{
		case EventType::LayerCreated:
		{
			CHANGE_POINTERS(m_Callbacks.created, func);
			break;
		}
		case EventType::LayerDeleted:
		{
			CHANGE_POINTERS(m_Callbacks.deleted, func);
			break;
		}
		case EventType::LayerReset:
		{
			CHANGE_POINTERS(m_Callbacks.reset, func);
			break;
		}
		case EventType::LayerDisabled:
		{
			CHANGE_POINTERS(m_Callbacks.disabled, func);
			break;
		}
		case EventType::LayerEnabled:
		{
			CHANGE_POINTERS(m_Callbacks.enabled, func);
			break;
		}
		case EventType::LayerCopied:
		{
			CHANGE_POINTERS(m_Callbacks.copied, func);
			break;
		}
		case EventType::LayerCopyAssigned:
		{
			CHANGE_POINTERS(m_Callbacks.copyassign, func);
			break;
		}
		case EventType::LayerMoved:
		{
			CHANGE_POINTERS(m_Callbacks.moved, func);
			break;
		}
		case EventType::LayerMoveAssigned:
		{
			CHANGE_POINTERS(m_Callbacks.moveassign, func);
			break;
		}
		case EventType::LayerActive:
		{
			CHANGE_POINTERS(m_Callbacks.active, func);
			break;
		}
		case EventType::LayerInactive:
		{
			CHANGE_POINTERS(m_Callbacks.inactive, func);
			break;
		}
		case EventType::LayerCleared:
		{
			CHANGE_POINTERS(m_Callbacks.cleared, func);
			break;
		}
		default:
		{
			OL_CORE_ERROR("Enter Valid LayerEvent type");
			assert(false);
		}
		}
		return;
	}

} // namespace Neural
} // namespace Soren