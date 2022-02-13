#include "olpch.h"
#include "Network.h"
#include "netData.h"

#include "Layer/Layer.h"
#include "Node/Node.h"

namespace Soren {
namespace Neural {

	void Network::Nodes(const size_t nodes)
	{
		DefaultNodeCount = nodes;
	}

	void Network::NodesDefaultBiasLimits(Bias_t min, Bias_t max)
	{
		m_BiasData.DownInit = true;
		m_BiasData.DownLimit = min;
		m_BiasData.UpInit = true;
		m_BiasData.UpLimit = max;
	}

	void Network::NodesDefaultBiasLimits(Bias_t max)
	{
		m_BiasData.UpInit = true;
		m_BiasData.UpLimit = max;
	}

	void Network::BiasData(Bias_t max, Bias_t min)
	{
		m_BiasData.UpInit = true; m_BiasData.UpLimit = max; m_BiasData.DownInit = true; m_BiasData.DownLimit = min;
	}

	void Network::BiasData(Bias_t max)
	{
		m_BiasData.UpInit = true; m_BiasData.UpLimit = max;
	}


	void Network::DeleteNode(const Node& node)
	{
		for (auto layer : m_Layers)
		{
			if (std::find(layer.m_Nodes.begin(), layer.m_Nodes.end(), node) != layer.m_Nodes.end())
			{
				layer.DeleteNode(node);
			}
		}
	}

	void Network::DeleteNode(size_t id)
	{
		for (auto layer : m_Layers)
		{
			if (std::find(layer.m_NodeIDs.begin(), layer.m_NodeIDs.end(), id) != layer.m_NodeIDs.end())
			{
				layer.DeleteNode(id);
			}
		}
	}

} // namespace Neural
} // namespace Soren