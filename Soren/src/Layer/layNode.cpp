#include "olpch.h"
#include "Log/Log.h"

#include "Layer/Layer.h"
#include "Node/Node.h"
#include "Link/Link.h"

namespace Soren {
namespace Neural {

	// Node function implementation

	Accessor<Node>& Layer::Nodes(size_t nodes, const std::string& name)
	{
		if (m_Locked) return LayNodes;

		if (nodes <= m_Nodes.size())
		{
			m_Nodes.erase(m_Nodes.begin() + nodes, m_Nodes.end());
			m_NodeIDs.erase(m_NodeIDs.begin() + nodes, m_NodeIDs.end());

			// set a certain threshhold where the vector is shrunk before which it is not shrunk
			//m_Nodes.shrink_to_fit();
			//m_NodeIDs.shrink_to_fit();

			return LayNodes;
		}

		m_Nodes.reserve(nodes);
		m_NodeIDs.reserve(nodes);
		
		for (int i = 0; i != nodes; i++)
		{
			m_Nodes.emplace_back(*this, name);
			m_NodeIDs.emplace_back(m_Nodes.back().m_NodeID);
		}
		return LayNodes;
	}

	Accessor<Node>& Layer::AddNodes(size_t nodes, const std::string& name)
	{
		if (m_Locked) return LayNodes;

		m_Nodes.reserve(m_Nodes.size() + nodes);
		m_NodeIDs.reserve(m_NodeIDs.size() + nodes);

		for (int i = 0; i != nodes; i++)
		{
			m_Nodes.emplace_back(*this, name);
			m_NodeIDs.emplace_back(m_Nodes.back().m_NodeID);
		}
		return LayNodes;
	}

	Node& Layer::GetNode(size_t nodeid)
	{
		return *std::find(m_Nodes.begin(), m_Nodes.end(), nodeid);
	}

	size_t Layer::FindNode(const Node& node)
	{
		if (*this != node)
		{
			OL_CORE_INFO("Node[{0}] not found in Layer[{1}]", node.m_NodeID, m_LayerID);
			return 0;
		}

		return std::distance(m_Nodes.begin(), std::find(m_Nodes.begin(), m_Nodes.end(), node));
	}

	void Layer::DeleteNode(size_t id)
	{
		if (m_Locked) return;

		std::vector<Node>::iterator pos = std::find(m_Nodes.begin(), m_Nodes.end(), id);
		if (pos != m_Nodes.end())
		{
			m_Nodes.erase(pos);
			return;
		}
		OL_CORE_ERROR("Node with ID: {0} not found", id);
		assert(false);
	}
	
	void Layer::DeleteNode(const Node& node)
	{
		if (m_Locked) return;

		std::vector<Node>::iterator pos = std::find(m_Nodes.begin(), m_Nodes.end(), node);
		if (pos != m_Nodes.end())
		{
			m_Nodes.erase(pos);
			return;
		}
		OL_CORE_ERROR("Node not found");
		assert(false);
	}

	void Layer::Put(Node& node)
	{
		// go through all links and look and get data and put it in links
		// if the InID is larger than the size, it wont work
		
		if (!m_InBuffer.initialized)
		{
			OL_CORE_WARN("InBuffer not initialized");
		}

		for (auto& link : node.m_Links)
		{
			link.m_InBuffer = m_InBuffer[link.m_InID].NodeValue;
		}
	}

	void Layer::Pull(const Node& node)
	{
		// should be looped from first node to last
		// or should the elements already be there then they are just modified
		if (!m_OutBuffer.initialized)
		{
			OL_CORE_WARN("OutBuffer not initialized");
		}

		m_OutBuffer.resize(m_Nodes.size());

		m_OutBuffer.emplace_back(node.m_NodeID, node.m_Value);
	}

} // namespace Neural
} // namespace Soren