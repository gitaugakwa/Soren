#include "olpch.h"
#include "Network/Network.h"

#include "Events/Event.h"
#include "Events/NetworkEvent.h"

#include "Layer/Layer.h"

#include "Input/Input.h"
#include "Output/Output.h"

namespace Soren {
namespace Neural {

	Network::Network()
	{
		m_Title = "Network";

		NetLayers = m_Layers;

		FileInputs = m_FileInputs;
		FileOutputs = m_FileOutputs;

		Bind();

		NetworkCreatedEvent event(m_Title);
		m_EventCallback(event);
	}

	Network::~Network()
	{
		NetworkDeletedEvent event(m_Title);
		m_EventCallback(event);

		deinit();
	}

	size_t Network::size() const
	{
		return m_Layers.size();
	}

	void Network::clear()
	{
		if (m_Locked) return;

		NetworkClearedEvent event(m_Title, m_Layers.size());
		m_EventCallback(event);

		m_Layers.clear();
	}

	void Network::reset()
	{
		if (m_Locked) return;
		// Reset to default state which has no layers
		NetworkResetEvent event(m_Title, m_Layers.size());
		m_EventCallback(event);

		m_Callbacks.Default();
		m_Events.clear();
		m_Layers.clear();
		// this will have to be for every fileinput vector fileoutput vector and streaminput streamoutput
		m_FileInputs.clear();
		m_FileOutputs.clear();
		SetStatus(StatusCategory::Idle);
		SetStatus(StatusCategory::Uninitialized);
	}

	void Network::init()
	{
		if (m_Locked) return;
		NetworkInitializedEvent event(m_Title);
		m_EventCallback(event);

		SetStatus(StatusCategory::Initialized);
	}

	void Network::deinit()
	{
		if (m_Locked) return;
	}

	bool Network::NetworkValid() const
	{
		if (m_FileInputs.empty())
		{
			OL_CORE_WARN("Network Has No Inputs");
		}

		if (m_FileOutputs.empty())
		{
			OL_CORE_WARN("Network Has No Outputs");
		}

		if (!Is(StatusCategory::Initialized))
		{
			OL_CORE_ERROR("Network Not Initialized");
			assert(false);
			return false;
		}

		return true;
	}


	Network::Network(const Network& arg)
		// maintain id so that Nodes still have the referenc to the same Network
		: m_Title		(arg.m_Title),
		m_Locked		(arg.m_Locked),
		m_EventCallback	(arg.m_EventCallback),
		m_Layers		(arg.m_Layers),
		m_Status		(arg.m_Status),
		m_PutFunc		(arg.m_PutFunc),
		m_PullFunc		(arg.m_PullFunc),
		m_FileInputs	(arg.m_FileInputs),
		m_FileOutputs	(arg.m_FileOutputs),
		m_BaseDir		(arg.m_BaseDir),
		m_Callbacks		(arg.m_Callbacks),
		m_Events		(arg.m_Events),
		m_LinkCounter	(arg.m_LinkCounter),
		m_NodeCounter	(arg.m_NodeCounter),
		m_LayerCounter	(arg.m_LayerCounter),
		m_LinkEvents	(arg.m_LinkEvents),
		m_LinkCallbacks	(arg.m_LinkCallbacks),
		m_LinkEventCallback(arg.m_LinkEventCallback)
	{
		// This is like assignment in the normal constructor
		// technically should be the same place
		// Reset the pointer to the new location in memory
		NetLayers = m_Layers;

		for (Layer& layer : m_Layers)
		{
			layer.m_Network = *this;
		}

		NetworkCopiedEvent event(m_Title, m_Layers.size());
		m_EventCallback(event);
	}

	Network::Network(Network&& arg) noexcept
		// maintain id so that Nodes still have the referenc to the same Network
		: m_Title		(std::move(arg.m_Title)),
		m_Locked		(std::exchange(arg.m_Locked, true)),
		m_EventCallback	(std::move(arg.m_EventCallback)),
		m_Layers		(std::move(arg.m_Layers)),
		m_Status		(std::move(arg.m_Status)),
		m_PutFunc		(std::move(arg.m_PutFunc)),
		m_PullFunc		(std::move(arg.m_PullFunc)),
		m_FileInputs	(std::move(arg.m_FileInputs)),
		m_FileOutputs	(std::move(arg.m_FileOutputs)),
		m_BaseDir		(std::move(arg.m_BaseDir)),
		m_Callbacks		(std::move(arg.m_Callbacks)),
		m_Events		(std::move(arg.m_Events)),
		m_LinkCounter	(std::move(arg.m_LinkCounter)),
		m_NodeCounter	(std::move(arg.m_NodeCounter)),
		m_LayerCounter	(std::move(arg.m_LayerCounter)),
		m_LinkEvents	(std::move(arg.m_LinkEvents)),
		m_LinkCallbacks	(std::move(arg.m_LinkCallbacks)),
		m_LinkEventCallback(std::move(arg.m_LinkEventCallback))
	{
		// This is like assignment in the normal constructor
		// Reset the pointer to the new location in memory
		NetLayers = m_Layers;

		for (Layer& layer : m_Layers)
		{
			layer.m_Network = *this;
		}

		NetworkMovedEvent event(m_Title, m_Layers.size());
		m_EventCallback(event);
	}

	Network& Network::operator=(const Network& arg)
		// IDs will be differnt cause if they were to be the same, then use copy constructor
		// don't thing we should copy title since it should also be used to describe the Network
	{
		if (*this == arg) return *this;

		NetworkCopyAssignedEvent event(arg.m_Title, m_Title, m_Layers.size());
		m_EventCallback(event);

		m_Title			= arg.m_Title;
		m_Locked		= arg.m_Locked;
		m_EventCallback	= arg.m_EventCallback;
		m_Layers		= arg.m_Layers;
		m_Status		= arg.m_Status;
		m_PutFunc		= arg.m_PutFunc;
		m_PullFunc		= arg.m_PullFunc;
		m_FileInputs	= arg.m_FileInputs;
		m_FileOutputs	= arg.m_FileOutputs;
		m_BaseDir		= arg.m_BaseDir;
		m_Callbacks		= arg.m_Callbacks;
		m_Events		= arg.m_Events;
		m_LinkCounter	= arg.m_LinkCounter;
		m_NodeCounter	= arg.m_NodeCounter;
		m_LayerCounter	= arg.m_LayerCounter;
		m_LinkEvents	= arg.m_LinkEvents;
		m_LinkCallbacks	= arg.m_LinkCallbacks;
		m_LinkEventCallback=arg.m_LinkEventCallback;

		// technically should be the same place
		// Reset the pointer to the new location in memory
		NetLayers = m_Layers;

		for (Layer& layer : m_Layers)
		{
			layer.m_Network = *this;
		}

		return *this;
	}

	Network& Network::operator=(Network&& arg) noexcept
	{
		// TODO: insert return statement here

		NetworkMoveAssignedEvent event(arg.m_Title, m_Title, m_Layers.size());
		m_EventCallback(event);

		m_Title			= std::move(arg.m_Title);
		m_Locked		= std::move(arg.m_Locked);
		m_EventCallback	= std::move(arg.m_EventCallback);
		m_Layers		= std::move(arg.m_Layers);
		m_Status		= std::move(arg.m_Status);
		m_PutFunc		= std::move(arg.m_PutFunc);
		m_PullFunc		= std::move(arg.m_PullFunc);
		m_FileInputs	= std::move(arg.m_FileInputs);
		m_FileOutputs	= std::move(arg.m_FileOutputs);
		m_BaseDir		= std::move(arg.m_BaseDir);
		m_Callbacks		= std::move(arg.m_Callbacks);
		m_Events		= std::move(arg.m_Events);
		m_LinkCounter	= std::move(arg.m_LinkCounter);
		m_NodeCounter	= std::move(arg.m_NodeCounter);
		m_LayerCounter	= std::move(arg.m_LayerCounter);
		m_LinkEvents	= std::move(arg.m_LinkEvents);
		m_LinkCallbacks	= std::move(arg.m_LinkCallbacks);
		m_LinkEventCallback=std::move(arg.m_LinkEventCallback);

		// technically should be the same place
		// Reset the pointer to the new location in memory
		NetLayers = m_Layers;

		for (Layer& layer : m_Layers)
		{
			layer.m_Network = *this;
		}

		return *this;
	}

	Layer& Network::operator[](size_t rhs)
	{
		return m_Layers[rhs];
	}

	bool Network::operator==(const Network& rhs) const
	{
		return (m_Layers		== rhs.m_Layers &&
				m_Callbacks		== rhs.m_Callbacks &&
				m_Events		== rhs.m_Events &&
				m_FileInputs	== rhs.m_FileInputs &&
				m_FileOutputs	== rhs.m_FileOutputs &&
				m_Locked		== rhs.m_Locked &&
				m_BaseDir		== rhs.m_BaseDir &&
				m_LinkCounter	== rhs.m_LinkCounter &&
				m_NodeCounter	== rhs.m_NodeCounter &&
				m_LayerCounter	== rhs.m_LayerCounter &&
				m_LinkEvents	== rhs.m_LinkEvents &&
				m_LinkCallbacks == rhs.m_LinkCallbacks &&
				m_Title			== rhs.m_Title);
	}

	bool Network::operator==(const Layer& rhs) const
	{
		return (std::find(m_Layers.begin(), m_Layers.end(), rhs) == m_Layers.end()) ? false : true;
	}

	bool Network::operator!=(const Network& rhs) const
	{
		return !(*this == rhs);
	}

	bool Network::operator!=(const Layer& rhs) const
	{
		return !(*this == rhs);
	}

	std::ostream& operator<<(std::ostream& os, const Network& Network)
	{
		return os << Network.str();
	}

} // namespace Neural
} // namespace Soren
