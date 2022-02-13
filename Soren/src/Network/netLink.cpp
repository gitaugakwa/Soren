#include "olpch.h"
#include "Network.h"

#include "Layer/Layer.h"

namespace Soren {
namespace Neural {

	void Network::Links(const Layer& source, Layer& output)
	{
		output.Links(source);
	}

	void Network::LinksDefaultWeightLimits(Weight_t min, const Weight_t& max)
	{
		m_WeightData.DownInit = true;
		m_WeightData.DownLimit = min;
		m_WeightData.UpInit = true;
		m_WeightData.UpLimit = max;
	}

	void Network::LinksDefaultWeightLimits(Weight_t max)
	{
		m_WeightData.UpInit = true;
		m_WeightData.UpLimit = max;
	}


	void Network::WeightData(Weight_t min, Weight_t max)
	{
		m_WeightData.UpInit = true; m_WeightData.UpLimit = max; m_WeightData.DownInit = true; m_WeightData.DownLimit = min;
	}

	inline void Network::WeightData(Weight_t max)
	{
		m_WeightData.UpInit = true; m_WeightData.UpLimit = max;
	}

} // namespace Neural
} // namespace Soren