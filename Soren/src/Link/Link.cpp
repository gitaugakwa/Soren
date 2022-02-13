#include "olpch.h"
#include "Link/Link.h"

#include <sstream>
#include <iomanip>

#include "Node/Node.h"
#include "Network/Network.h"

#include "Random/Random.h"

#include "Events/Event.h"
#include "Events/LinkEvent.h"


namespace Soren {
namespace Neural {

	Link::Link(Node& node, const Node& input)
		: m_InID(input.id()), m_OutID(node.id()), m_LinkID(++node.network().m_LinkCounter), m_Node(node)
	{
		if (network().WeightData().DownInit)
			m_Weight = Random::Number<Weight_t>(network().WeightData().DownLimit,
												network().WeightData().UpLimit);
		else if (network().WeightData().UpInit)
			m_Weight = Random::Number<Weight_t>(network().WeightData().UpLimit);
		else
		{
			// OL_CORE_WARN("No Default Link Weight Limits Set");
			m_Weight = Random::Number<Weight_t>();
		}

		Bind();

		LinkCreatedEvent event(m_LinkID, m_InID, m_OutID);
		network().m_LinkEventCallback(event);
	}

	Link::~Link()
	{
		LinkDeletedEvent event(m_LinkID, m_InID, m_OutID);
		network().m_LinkEventCallback(event);
	}

	void Link::clear()
	{
		if (m_Locked) return;

		LinkClearedEvent event(m_LinkID, m_InID, m_OutID, m_Weight);
		network().m_LinkEventCallback(event);

		// Reset Activation function if stored by Link
		m_Weight = NULL;
	}

	void Link::halt()
	{
		LinkDisabledEvent event(m_LinkID, m_InID, m_OutID);
		network().m_LinkEventCallback(event);

		m_Running = FALSE;
	}

	void Link::resume()
	{
		m_Running = TRUE;

		LinkEnabledEvent event(m_LinkID, m_InID, m_OutID);
		network().m_LinkEventCallback(event);
	}

	void Link::reset()
	{
		LinkResetEvent event(m_LinkID, m_InID, m_OutID, m_Weight);
		network().m_LinkEventCallback(event);
	}

	std::string Link::str(const std::string& linkpref, const std::string& linksuf) const
	{
		std::stringstream ss;

		ss << linkpref;
		ss << "Link[" << m_LinkID << "]";
		ss << " -> {Node[" << m_InID << "]";
		ss << " -> Node[" << m_OutID << "]}";
		ss << " Weight: " << std::setprecision(std::numeric_limits<Weight_t>::digits10 + 1) << m_Weight;
		if (m_InBuffer)
		{
			ss << " InBuffer: " << m_InBuffer;
		}
		ss << linksuf;
		ss << std::endl;

		return ss.str();
	}

	Link::Link(const Link& arg)
		:	m_LinkID	(arg.m_LinkID),
			m_Weight	(arg.m_Weight),
			m_Running	(arg.m_Running),
			m_Locked	(arg.m_Locked),
			m_Node		(arg.m_Node),
			m_InID		(arg.m_InID),
			m_InBuffer	(arg.m_InBuffer),
			m_OutID		(arg.m_OutID)
	{
		LinkCopiedEvent event(m_LinkID, m_InID, m_OutID, m_Weight);
		network().m_LinkEventCallback(event);
	}

	Link::Link(Link&& arg) noexcept
		:	m_LinkID	(arg.m_LinkID),
			m_Weight	(std::exchange(arg.m_Weight,0)),
			m_Running	(std::exchange(arg.m_Running,false)),
			m_Locked	(std::exchange(arg.m_Locked,true)),
			m_Node		(arg.m_Node),
			m_InID		(std::exchange(arg.m_InID,0)),
			m_InBuffer	(std::exchange(arg.m_InBuffer,0)),
			m_OutID		(std::exchange(arg.m_OutID,0))
	{
		LinkMovedEvent event(m_LinkID, m_InID, m_OutID, m_Weight);
		network().m_LinkEventCallback(event);
	}

	Link& Link::operator=(const Link& arg)
	{
		if (*this == arg) return *this;

		LinkCopyAssignedEvent event(arg.m_LinkID, m_LinkID, arg.m_InID, arg.m_OutID, m_InID, m_OutID, arg.m_Weight, m_Weight);
		network().m_LinkEventCallback(event);

		m_Weight	= arg.m_Weight;
		m_Running	= arg.m_Running;
		m_Locked	= arg.m_Locked;
		m_Node		= arg.m_Node;
		m_InID		= arg.m_InID;
		m_InBuffer	= arg.m_InBuffer;
		m_OutID		= arg.m_OutID;

		return *this;
	}

	Link& Link::operator=(Link&& arg) noexcept
	{
		LinkMoveAssignedEvent event(arg.m_LinkID, m_LinkID, arg.m_InID, arg.m_OutID, m_InID, m_OutID, arg.m_Weight, m_Weight);
		network().m_LinkEventCallback(event);

		m_Weight	= std::move(arg.m_Weight);
		m_Running	= std::move(arg.m_Running);
		m_Locked	= std::move(arg.m_Locked);
		m_Node		= std::move(arg.m_Node);
		m_InID		= std::move(arg.m_InID);
		m_InBuffer	= std::move(arg.m_InBuffer);
		m_OutID		= std::move(arg.m_OutID);

		return *this;
	}

	bool Link::operator==(const Link& rhs) const
	{
		return (m_LinkID	== rhs.m_LinkID &&
				m_Weight	== rhs.m_Weight &&
				m_Running	== rhs.m_Running &&
				m_Locked	== rhs.m_Locked &&
				// m_Node		== rhs.m_Node &&
				m_InID		== rhs.m_InID &&
				m_OutID		== rhs.m_OutID);
	}

	bool Link::operator==(const Node& rhs) const
	{
		return (*this).m_InID == rhs.id();
	}

	bool Link::operator!=(const Link& rhs) const
	{
		return !(*this == rhs);
	}

	bool Link::operator!=(const Node& rhs) const
	{
		return !(*this == rhs);
	}

	std::ostream& operator<<(std::ostream& os, const Link& link)
	{
		return os << link.str();
	}

} // namespace Neural
} // namespace Soren
