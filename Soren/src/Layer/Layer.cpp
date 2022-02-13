#include "olpch.h"
#include "Layer/Layer.h"

#include "Node/Node.h"
#include "Network/Network.h"

#include "Events/Event.h"
#include "Events/LayerEvent.h"

namespace Soren {
namespace Neural {

	Layer::Layer(Network& network, const std::string& title)
		: m_LayerID(++network.m_LayerCounter), m_Title(title), m_Network(network)
	{
		Bind();

		LayNodes = m_Nodes;

		LayerCreatedEvent event(m_LayerID, m_Title);
		m_Network.m_LayerEventCallback(event);
	}


	Layer::~Layer()
	{
		LayerDeletedEvent event(m_LayerID, m_Title);
		m_Network.m_LayerEventCallback(event);
	}

	size_t Layer::size() const
	{
		return m_Nodes.size();
	}

	size_t Layer::pos() const
	{
		return m_Network.FindLayer(*this);
	}

	void Layer::clear()
	{
		if (m_Locked) return;

		LayerClearedEvent event(m_LayerID, m_Title, m_Nodes.size());
		m_Network.m_LayerEventCallback(event);

		m_Nodes.clear();
	}

	Network& Layer::network() const
	{
		return m_Network;
	}


	Layer::Layer(const Layer& arg)
		// maintain id so that Nodes still have the referenc to the same Layer
		:	m_LayerID		(arg.m_LayerID),
			m_Title			(arg.m_Title),
			m_Locked		(arg.m_Locked),
			m_Network		(arg.m_Network),
			m_Nodes			(arg.m_Nodes),
			m_InBuffer		(arg.m_InBuffer),
			m_OutBuffer		(arg.m_OutBuffer),
			m_CurrentCall	(arg.m_CurrentCall),
			m_NodeIDs		(arg.m_NodeIDs),
			m_Callbacks		(arg.m_Callbacks),
			m_Events		(arg.m_Events)
	{
		// This is like assignment in the normal constructor
		// technically should be the same place
		// Reset the pointer to the new location in memory
		LayNodes			= m_Nodes;

		for (Node& node : m_Nodes)
		{
			node.m_Layer = *this;
		}

		LayerCopiedEvent event(m_LayerID, m_Title, m_Nodes.size());
		m_Network.m_LayerEventCallback(event);
	}

	Layer::Layer(Layer&& arg) noexcept
		// maintain id so that Nodes still have the referenc to the same Layer
		:	m_LayerID		(arg.m_LayerID),
			m_Title			(std::move(arg.m_Title)),
			m_Locked		(std::exchange(arg.m_Locked, true)),
			m_Network		(arg.m_Network),
			m_Nodes			(std::move(arg.m_Nodes)),
			m_InBuffer		(std::move(arg.m_InBuffer)),
			m_OutBuffer		(std::move(arg.m_OutBuffer)),
			m_CurrentCall	(std::exchange(arg.m_CurrentCall,0)),
			m_NodeIDs		(std::move(arg.m_NodeIDs)),
			m_Callbacks		(std::move(arg.m_Callbacks)),
			m_Events		(std::move(arg.m_Events))
	{
		// This is like assignment in the normal constructor
		// Reset the pointer to the new location in memory
		LayNodes			= m_Nodes;

		for (Node& node : m_Nodes)
		{
			node.m_Layer = *this;
		}

		LayerMovedEvent event(m_LayerID, m_Title, m_Nodes.size());
		m_Network.m_LayerEventCallback(event);
	}

	Layer& Layer::operator=(const Layer& arg)
		// IDs will be differnt cause if they were to be the same, then use copy constructor
		// don't thing we should copy title since it should also be used to describe the layer
	{
		if (*this == arg) return *this;

		LayerCopyAssignedEvent event(arg.m_LayerID, m_LayerID, arg.m_Title, m_Title, m_Nodes.size());
		m_Network.m_LayerEventCallback(event);

		m_Title			= arg.m_Title;
		m_Locked		= arg.m_Locked;
		m_Network		= arg.m_Network;
		m_Nodes			= arg.m_Nodes;
		m_InBuffer		= arg.m_InBuffer;
		m_OutBuffer		= arg.m_OutBuffer;
		m_CurrentCall	= arg.m_CurrentCall;
		m_NodeIDs		= arg.m_NodeIDs;
		m_Callbacks		= arg.m_Callbacks;
		m_Events		= arg.m_Events;

		// technically should be the same place
		// Reset the pointer to the new location in memory
		LayNodes		= m_Nodes;

		for (Node& node : m_Nodes)
		{
			node.m_Layer = *this;
		}

		return *this;
	}

	Layer& Layer::operator=(Layer&& arg) noexcept
	{
		// TODO: insert return statement here

		LayerMoveAssignedEvent event(arg.m_LayerID, m_LayerID, arg.m_Title, m_Title, m_Nodes.size());
		m_Network.m_LayerEventCallback(event);

		m_Title			= std::move(arg.m_Title);
		m_Locked		= std::move(arg.m_Locked);
		m_Network		= std::move(arg.m_Network);
		m_Nodes			= std::move(arg.m_Nodes);
		m_InBuffer		= std::move(arg.m_InBuffer);
		m_OutBuffer		= std::move(arg.m_OutBuffer);
		m_CurrentCall	= std::move(arg.m_CurrentCall);
		m_NodeIDs		= std::move(arg.m_NodeIDs);
		m_Callbacks		= std::move(arg.m_Callbacks);
		m_Events		= std::move(arg.m_Events);

		// technically should be the same place
		// Reset the pointer to the new location in memory
		LayNodes		= m_Nodes;

		for (Node& node : m_Nodes)
		{
			node.m_Layer = *this;
		}

		return *this;
	}

	Node& Layer::operator[](size_t rhs)
	{
		return m_Nodes[rhs];
	}

	bool Layer::operator==(const Layer& rhs) const
	{
		return (m_LayerID	== rhs.m_LayerID &&
				m_NodeIDs	== rhs.m_NodeIDs &&
				m_Nodes		== rhs.m_Nodes &&
				m_Callbacks	== rhs.m_Callbacks &&
				m_Events	== rhs.m_Events &&
				m_InBuffer	== rhs.m_InBuffer &&
				m_OutBuffer	== rhs.m_OutBuffer &&
				m_Locked	== rhs.m_Locked &&
				// m_Network	== rhs.m_Network &&
				m_Title		== rhs.m_Title);
	}

	bool Layer::operator==(const Node& rhs) const
	{
		return (std::find(m_NodeIDs.begin(), m_NodeIDs.end(), rhs.m_NodeID) == m_NodeIDs.end()) ? false : true;
	}

	bool Layer::operator==(size_t rhs) const
	{
		return m_LayerID == rhs;
	}

	bool Layer::operator!=(const Layer& rhs) const
	{
		return !(*this == rhs);
	}

	bool Layer::operator!=(const Node& rhs) const
	{
		return !(*this == rhs);
	}

	bool Layer::operator!=(size_t rhs) const
	{
		return !(*this == rhs);
	}

	std::ostream& operator<<(std::ostream& os, const Layer& layer)
	{
		return os << layer.str();
	}

	bool Layer::operator<(const Layer& rhs) const
	{
		// Check if the layer provided is below the current layer
		return m_Network.FindLayer(rhs) < m_Network.FindLayer(*this);
	}

	bool Layer::operator>(const Layer& rhs) const
	{
		// Check if the layer provided is above the current layer
		return m_Network.FindLayer(rhs) > m_Network.FindLayer(*this);
	}

	bool Layer::operator<=(const Layer& rhs) const
	{
		return m_Network.FindLayer(rhs) <= m_Network.FindLayer(*this);
	}

	bool Layer::operator>=(const Layer& rhs) const
	{
		return m_Network.FindLayer(rhs) > m_Network.FindLayer(*this);
	}

} // namespace Neural
} // namespace Soren
