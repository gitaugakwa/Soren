#include "olpch.h"
#include "Network/Network.h"

#include "Events/NetworkEvent.h"

namespace Soren {
namespace Neural {

	// Status function

	void Network::SetStatus(const StatusCategory& status)
	{
		// Getting previous status

		std::string prev = m_Status.str();

		// Setting current status
		m_Status.SetStatus(status);

		std::string curr = m_Status.str();

		NetworkStatusChangeEvent event(m_Title, prev, curr);
		m_EventCallback(event);
	}

} // namespace Neural
} // namespace Soren