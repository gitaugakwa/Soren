#include "olpch.h"
#include "Link/Link.h"

#include "Node/Node.h"
#include "Layer/Layer.h"
#include "Network/Network.h"

namespace Soren {
namespace Neural {

	Network& Link::network()
	{
		return m_Node.network();
	}


} // namespace Neural
} // namespace Soren