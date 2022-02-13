#include "olpch.h"
#include "Node/Node.h"

#include "Network/Network.h"
#include "Layer/Layer.h"
#include "Link/Link.h"

namespace Soren {
namespace Neural {

	void Node::DeleteLink(const Node& node)
	{
		if (m_Locked) return;

		std::vector<Link>::iterator pos = std::find(m_Links.begin(), m_Links.end(), node);
		if (pos != m_Links.end())
		{
			m_Links.erase(pos);
		}
		OL_CORE_ERROR("Link not found");
		assert(false);
	}

	void Node::DeleteLink(const Link& link)
	{
		if (m_Locked) return;

		std::vector<Link>::iterator pos = std::find(m_Links.begin(), m_Links.end(), link);
		if (pos != m_Links.end())
		{
			m_Links.erase(pos);
		}
		OL_CORE_ERROR("Link not found");
		assert(false);
	}

	bool Node::LinkCreated(const Node& node) const
	{
		for (const Link& link : m_Links)
		{
			if (link == node)
				return true;
		}
		return false;
	}

	//std::vector<Link*>& Node::Links(const std::initializer_list<Node&>& nodes)
	//{
	//	m_Links.reserve(m_Links.capacity() + nodes.size());
	//	NodeLinks.reserve(NodeLinks.capacity() + nodes.size());
	//	for (Node& node : nodes)
	//	{
	//		// check is the link has already been created
	//		m_Links.emplace_back(node, *this);
	//		NodeLinks.insert(NodeLinks.end(), &m_Links.back());
	//	}
	//	return NodeLinks;
	//}

	// currently limit it to nodes below current layer

	// Links will never clear previous Links in Node when called unlike Nodes and Layers

	Accessor<Link>& Node::Links(const std::initializer_list<Node*>& nodes)
	{
		if (m_Locked) return NodeLinks;

		m_Links.reserve(m_Links.size() + nodes.size());
		for (Node* node : nodes)
		{
			if (LinkCreated(*node))
			{
				OL_CORE_WARN("Node[{0}] already has Link with Node[{1}]", m_NodeID, node->m_NodeID);
				OL_CORE_WARN("No New Link will be created");
				continue;
			}
			m_Links.emplace_back(*this, *node);
		}
		return NodeLinks;
	}

	Accessor<Link>& Node::Links(const Node& node)
	{
		if (m_Locked) return NodeLinks;

		if (LinkCreated(node))
		{
			OL_CORE_WARN("Node[{0}] already has Link with Node[{1}]", m_NodeID, node.m_NodeID);
			OL_CORE_WARN("No New Link will be created");
			return NodeLinks;
		}
		m_Links.emplace_back(*this, node);
		return NodeLinks;
	}

	Accessor<Link>& Node::Links(const Node* node)
	{
		if (m_Locked) return NodeLinks;

		if (LinkCreated(*node))
		{
			OL_CORE_WARN("Node[{0}] already has Link with Node[{1}]", m_NodeID, node->m_NodeID);
			OL_CORE_WARN("No New Link will be created");
			return NodeLinks;
		}
		m_Links.emplace_back(*this, *node);
		return NodeLinks;
	}

	Accessor<Link>& Node::Links(const Layer& layer)
	{
		if (m_Locked) return NodeLinks;

		// Allow links between layers in different positions
		if (network().FindLayer(layer) < network().FindLayer(m_LayerID))
		{
			m_Links.reserve(m_Links.size() + layer.size());

			for (const Node& node : layer.m_Nodes)
			{
				if (LinkCreated(node))
				{
					OL_CORE_WARN("Node[{0}] already has Link with Node[{1}]", m_NodeID, node.m_NodeID);
					OL_CORE_WARN("No New Link will be created");
					continue;
				}
				m_Links.emplace_back(*this, node);
			}
		}
		else
		{
			OL_CORE_TRACE("Unable to link");
			OL_CORE_TRACE("Layer{{0}} is above the current layer{{1}}", network().FindLayer(layer), network().FindLayer(m_LayerID));
		}
		return NodeLinks;
	}

	Accessor<Link>& Node::Links(const Layer* layer)
	{
		if (m_Locked) return NodeLinks;

		// Allow links between layers in different positions
		if (network().FindLayer(*layer) < network().FindLayer(m_LayerID))
		{
			m_Links.reserve(m_Links.size() + layer->size());

			for (const Node& node : layer->m_Nodes)
			{
				if (LinkCreated(node))
				{
					OL_CORE_WARN("Node[{0}] already has Link with Node[{1}]", m_NodeID, node.m_NodeID);
					OL_CORE_WARN("No New Link will be created");
					continue;
				}
				m_Links.emplace_back(*this, node);
			}
		}
		else
		{
			OL_CORE_TRACE("Unable to link");
			OL_CORE_TRACE("Layer{{0}} is above the current layer{{1}}", network().FindLayer(*layer), network().FindLayer(m_LayerID));
		}
		return NodeLinks;
	}

	Link& Node::GetLink(const Node& node)
	{
		return *std::find(m_Links.begin(), m_Links.end(), node);
	}

	Node_t Node::Resolve()
	{
		Node_t raw = 0;
		for (auto& link : m_Links)
		{
			// InBuffer should be linked to the in node value
			raw += (link.m_InBuffer * link.m_Weight);
		}
		return raw;
	}

} // namespace Neural
} // namespace Soren