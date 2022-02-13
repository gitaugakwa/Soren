#include "olpch.h"
#include "Node/Node.h"

#include "Layer/Layer.h"
#include "Network/Network.h"

namespace Soren {
namespace Neural {

	Network& Node::network()
	{
		return m_Layer.network();
	}
} // namespace Neural
} // namespace Soren