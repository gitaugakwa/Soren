#pragma once
#include "Core.h"
#include "Base.h"

#include "Status/Status.h"

#include "Log/Log.h"
#include "Events/Event.h"

#include "nlohmann/json.hpp"

namespace Soren {

	template <typename T>
	class FileOutput;
	template <typename T>
	class FileInput;

	namespace Neural {

		template<typename TValue, typename TBias, typename TWeight>
		class Network;

		template<typename TValue = double, typename TTarget = TValue, typename TBias = TValue, typename TWeight = TValue>
		class SOREN_API Train {
		public:
			Train(std::shared_ptr<Network<TValue, TBias, TWeight>> network, double scale = -1, size_t epochs = 5)
				: mNetwork(network), mScale(scale), mEpochs(epochs) {
			}

			struct Result {
			
			};

			struct BatchResult {
				
			};

			

			//template<typename TInput>
			



		private:
			std::shared_ptr<Loss::Loss<TValue, TTarget>> mLoss{ std::make_shared<Soren::Loss::Loss<TValue, TTarget>>(Loss::CategoricalCrossentropy<TValue, TTarget>) };
			std::shared_ptr<Network<TValue, TBias, TWeight>> mNetwork;
			double mScale{};
			size_t mEpochs{};
		};




	} // namespace Train
} // namespace Soren