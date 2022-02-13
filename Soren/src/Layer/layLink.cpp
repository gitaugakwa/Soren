#include "olpch.h"
#include "Log/Log.h"

#include "Network/Network.h"
#include "Layer/Layer.h"
#include "Node/Node.h"
#include "Link/Link.h"

namespace Soren {
namespace Neural {

	void Layer::Links(const Node& node)
	{
		if (m_Locked) return;

		for (Node& currnode : m_Nodes)
		{
			currnode.Links(node);
		}
	}

	void Layer::Links(const Layer& layer)
	{
		if (m_Locked) return;

		if (m_Network.FindLayer(layer) < m_Network.FindLayer(*this))
		{
			for (Node& node : m_Nodes)
			{
				node.Links(layer);
			}
		}
		else
		{
			OL_CORE_TRACE("Unable to link");
			OL_CORE_TRACE("Layer{{0}} is above the current layer{{1}}", m_Network.FindLayer(layer), m_Network.FindLayer(*this));
		}
	}

} // namespace Neural
} // namespace Soren