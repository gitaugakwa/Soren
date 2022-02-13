#include "olpch.h"
#include "Activation.h"
#include "ThreadPool.h"
#include "Timer.h"
#include "Base.h"

#include "Train.h"

#include "Network/Network.h"
#include "Layer/Layer.h"
#include "Node/Node.h"
#include "Link/Link.h"

namespace Soren {
namespace Neural {

	Train::Train(size_t iterations, size_t threads)
		: m_Iterations(iterations), m_Threads(threads) { }

	void Train::Gradient(Gradient_t gradient)
	{
		m_Gradient.Initialized = true;
		m_Gradient.Gradient = gradient;
	}

	void Train::Variance(Variance_t variance)
	{
		m_Variance.Initialized = true;
		m_Variance.Variance = variance;
	}

	void Train::Activation(const Activation::ActivationFunc& activation)
	{
		m_Activation.Initialized = true;
		m_Activation.Activation = activation;
	}

	}
}