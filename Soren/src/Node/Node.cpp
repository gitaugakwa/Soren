#include "olpch.h"
#include "Node/Node.h"

#include <sstream>
#include <iomanip>

#include "Network/Network.h"
#include "Layer/Layer.h"
#include "Link/Link.h"

#include "Random/Random.h"

#include "Events/Event.h"
#include "Events/NodeEvent.h"

namespace Soren {
namespace Neural {

	Node::Node(Layer& layer, const std::string& name)
		: m_LayerID(layer.id()), m_NodeID(++layer.network().m_NodeCounter), m_Title(name), m_Layer(layer)
	{
		// what about setting it so a specific value???
		if (network().BiasData().DownInit)
			m_Bias = Random::Number<Bias_t>(network().BiasData().DownLimit, network().BiasData().UpLimit);
		else if (network().BiasData().UpInit)
			m_Bias = Random::Number<Bias_t>(network().BiasData().UpLimit);
		else
		{
			// OL_CORE_WARN("No Default Node Bias Limits Set");
			m_Bias = Random::Number<Bias_t>();
		}

		Bind();

		NodeLinks = m_Links;

		NodeCreatedEvent event(m_NodeID, m_Title, m_LayerID);
		network().m_NodeEventCallback(event);
	}

	Node::~Node()
	{
		NodeDeletedEvent event(m_NodeID, m_Title, m_LayerID);
		network().m_NodeEventCallback(event);
	}

	size_t Node::size() const
	{
		return m_Links.size();
	}

	void Node::clear()
	{
		if (m_Locked) return;

		NodeClearedEvent event(m_NodeID, m_Title, (unsigned int)m_Links.size());
		network().m_NodeEventCallback(event);

		m_Links.clear();
	}

	void Node::halt()
	{
		if (m_Locked) return;

		NodeDisabledEvent event(m_NodeID, m_Title);
		network().m_NodeEventCallback(event);

		m_Running = FALSE;
	}

	void Node::resume()
	{
		if (m_Locked) return;

		m_Running = TRUE;

		NodeEnabledEvent event(m_NodeID, m_Title);
		network().m_NodeEventCallback(event);
	}

	void Node::reset()
	{
		if (m_Locked) return;

		NodeResetEvent event(m_NodeID, m_Title, m_Links.size());
		network().m_NodeEventCallback(event);

		m_Links.clear();
	}

	Layer& Node::layer() const
	{
		return m_Layer;
	}

	Node::Node(const Node& arg)
		:	m_NodeID		(arg.m_NodeID),
			m_LayerID		(arg.m_LayerID),
			m_Title			(arg.m_Title),
			m_Running		(arg.m_Running),
			m_Locked		(arg.m_Locked),
			m_Layer			(arg.m_Layer),
			m_Links			(arg.m_Links),
			m_Value			(arg.m_Value),
			m_Bias			(arg.m_Bias),
			m_Callbacks		(arg.m_Callbacks),
			m_Events		(arg.m_Events)
	{
		// This is like assignment in the normal constructor
		// technically should be the same place
		// Reset the pointer to the new location in memory
		NodeLinks = m_Links;

		for (Link& link : m_Links)
		{
			link.m_Node = *this;
		}

		NodeCopiedEvent event(m_NodeID, m_Title, m_Links.size());
		network().m_NodeEventCallback(event);
	}

	Node::Node(Node&& arg) noexcept
		:	m_NodeID		(arg.m_NodeID),
			m_LayerID		(arg.m_LayerID),
			m_Title			(std::move(arg.m_Title)),
			m_Running		(std::exchange(arg.m_Running,false)),
			m_Locked		(std::exchange(arg.m_Locked,true)),
			m_Layer			(arg.m_Layer),
			m_Links			(std::move(arg.m_Links)),
			m_Value			(std::exchange(arg.m_Value,0)),
			m_Bias			(std::exchange(arg.m_Bias,0)),
			m_Callbacks		(std::move(arg.m_Callbacks)),
			m_Events		(std::move(arg.m_Events))
		// should there be a layerID so that the node is not linked to anything
	{
		// This is like assignment in the normal constructor
		// technically should be the same place
		// Reset the pointer to the new location in memory
		NodeLinks = m_Links;

		for (Link& link : m_Links)
		{
			link.m_Node = *this;
		}

		NodeMovedEvent event(m_NodeID, m_Title, (unsigned int)m_Links.size());
		network().m_NodeEventCallback(event);
	}
	Node& Node::operator=(const Node& arg)
	{
		if (*this == arg) return *this;

		NodeCopyAssignedEvent event(arg.m_LayerID, m_LayerID, arg.m_Title, m_Title, m_Links.size());
		network().m_NodeEventCallback(event);

		m_Title			= arg.m_Title;
		m_Running		= arg.m_Running;
		m_Locked		= arg.m_Locked;
		m_Layer			= arg.m_Layer;
		m_Links			= arg.m_Links;
		m_Value			= arg.m_Value;
		m_Bias			= arg.m_Bias;
		m_Callbacks		= arg.m_Callbacks;
		m_Events		= arg.m_Events;

		// This is like assignment in the normal constructor
		// technically should be the same place
		// Reset the pointer to the new location in memory
		NodeLinks = m_Links;

		for (Link& link : m_Links)
		{
			link.m_Node = *this;
		}

		return *this;
	}
	Node& Node::operator=(Node&& arg) noexcept
	{
		NodeMoveAssignedEvent event(arg.m_LayerID, m_LayerID, arg.m_Title, m_Title, m_Links.size());
		network().m_NodeEventCallback(event);

		m_Title			= std::move(arg.m_Title);
		m_Running		= std::move(arg.m_Running);
		m_Locked		= std::move(arg.m_Locked);
		m_Layer			= std::move(arg.m_Layer);
		m_Links			= std::move(arg.m_Links);
		m_Value			= std::move(arg.m_Value);
		m_Bias			= std::move(arg.m_Bias);
		m_Callbacks		= std::move(arg.m_Callbacks);
		m_Events		= std::move(arg.m_Events);

		// Reset the pointer to the new location in memory
		NodeLinks = m_Links;

		for (Link& link : m_Links)
		{
			link.m_Node = *this;
		}

		return *this;
	}

	Link& Node::operator[](size_t rhs)
	{
		return m_Links[rhs];
	}

	bool Node::operator==(const Node& rhs) const
	{
		return (m_NodeID	== rhs.m_NodeID &&
				m_LayerID	== rhs.m_LayerID &&
				m_Links		== rhs.m_Links &&
				m_Value		== rhs.m_Value &&
				m_Callbacks	== rhs.m_Callbacks &&
				m_Events	== rhs.m_Events &&
				m_Running	== rhs.m_Running &&
				m_Locked	== rhs.m_Locked &&
				// m_Layer		== rhs.m_Layer &&
				m_Title		== rhs.m_Title);
	}

	bool Node::operator==(size_t rhs) const
	{
		return (*this).m_NodeID == rhs;
	}

	bool Node::operator!=(const Node& rhs) const
	{
		return !(*this == rhs);
	}

	bool Node::operator!=(size_t rhs) const
	{
		return !(*this == rhs);
	}

	std::ostream& operator<<(std::ostream& os, const Node& e)
	{
		return os << e.str();
	}
} // namespace Neural
} // namespace Soren
