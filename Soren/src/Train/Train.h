#pragma once
#include "Activation.h"
#include "ThreadPool.h"
#include "Timer.h"
#include "Base.h"

#include "Network/Network.h"
#include "Layer/Layer.h"
#include "Node/Node.h"
#include "Link/Link.h"

namespace Soren {
namespace Neural {

	class SOREN_API Train {

	public:
		Train(size_t iterations = 10, size_t threads = 20);
		~Train() = default;

		bool complete() noexcept { return m_CurrentIteration == m_Iterations; }


		// Gradients
		void Gradient(Gradient_t gradient);
		inline GradientData Gradient() const { return m_Gradient; }

		// Variance
		void Variance(Variance_t variance);
		inline VarianceData Variance() const { return m_Variance; }

		// Activation Functions
		void Activation(const Soren::Activation::ActivationFunc& activation);
		inline ActivationData Activation() const { return m_Activation; }

		// Threads
		inline ThreadData& Threads() { return m_Threads; }

		// Losses
		inline void AddLoss(double loss) { m_Losses.emplace_back(loss); }

		// Iterations
		inline void Iterations(size_t iterations) { m_Iterations = iterations; }
		inline size_t CurrentIteration() const { return m_CurrentIteration; }
		inline void NextIteraton() { ++m_CurrentIteration; }

		// Operators
		operator bool() noexcept { return !complete(); }

	private:

		ActivationData m_Activation;
		GradientData m_Gradient;
		VarianceData m_Variance;
		ThreadData m_Threads;

		size_t m_CurrentIteration = 0;
		size_t m_Iterations = 0;

		std::vector<double> m_Losses;

	};
	}
}
