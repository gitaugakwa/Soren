#pragma once
#include "Core.h"
#include "Log/Log.h"
#include <math.h>

#include "Network/Network.h"

#include "Eigen/Core"


namespace Soren {

	namespace Optimizer {

		namespace Function {

			template<typename TValue, typename TBias = TValue, typename TWeight = TValue>
			static constexpr void GradientDescent(std::shared_ptr<Neural::Network<TValue, TBias, TWeight>>& network, const Eigen::Ref<const Eigen::MatrixX<TValue>>& DLossDLayerActivation,size_t epoch, double learning_rate)
			{
				epoch;
				Eigen::MatrixX<TValue> mDActivatedDValue;
				//Eigen::MatrixX<TValue> mDActivatedDInput;
				//Eigen::MatrixX<TValue> mDLossDWeight;
				Eigen::MatrixX<TValue> mDLossDActivated;
				Eigen::MatrixX<TValue> mDLossDInput{ DLossDLayerActivation };

				std::vector<Eigen::MatrixX<TValue>> mDActivatedDWeight;
				std::vector<Eigen::MatrixX<TValue>> mDLossDWeight;
				std::vector<Eigen::MatrixX<TValue>> mDActivatedDInput;
				//std::vector<std::shared_ptr<Eigen::MatrixX<TValue>>> DLossDWeight;
				std::vector<std::shared_ptr<Eigen::MatrixX<TValue>>> mDLayerActivationDActivated;

				for (auto layer = network->layers().rbegin(); layer != std::prev(network->layers().rend(), 1); layer++) {
					//SOREN_CORE_INFO(mDLossDInput);
					// DLayerActivation/DActivated
					mDLayerActivationDActivated = ((*layer)->layerActivation())->prime((*layer)->activated());
					mDLossDActivated.noalias() = Eigen::MatrixX<TValue>(mDLossDInput.rows(), mDLossDInput.cols());
					// DLossDActivated
					std::transform(std::execution::par_unseq, mDLayerActivationDActivated.begin(), mDLayerActivationDActivated.end(), mDLossDInput.colwise().begin(), mDLossDActivated.colwise().begin(), [](const auto& DLayerActivationDActivated, const auto& DLossDInput) {
						//SOREN_CORE_INFO(*DLayerActivationDActivated);
						return (*DLayerActivationDActivated) * DLossDInput;
						});
					//mDLossDActivated = mDLossDActivatedColwise;

					// DActivated/DValue
					mDActivatedDValue = ((*layer)->weighted().array().colwise() + (*layer)->biases().array()).unaryExpr([&](auto val) { return ((*layer)->activation())->prime(val); });

					// DActivated/DWeight
					//auto Inputs = (*layer)->inputs();
					mDActivatedDWeight.resize(mDActivatedDValue.cols());
					std::transform(std::execution::par_unseq, mDActivatedDValue.colwise().begin(), mDActivatedDValue.colwise().end(), (*layer)->inputs().colwise().begin(), mDActivatedDWeight.begin(), [](const auto& mDActivatedDValue, const auto& inputs) {
						return mDActivatedDValue * inputs.transpose();
						});

					// DLoss/DWeight
					mDLossDWeight.resize(mDActivatedDWeight.size());
					std::transform(std::execution::par_unseq, mDActivatedDWeight.begin(), mDActivatedDWeight.end(), mDLossDActivated.colwise().begin(), mDLossDWeight.begin(), [](const auto& DActivatedDWeight, const auto& DLossDActivated) {
						return DActivatedDWeight.array().colwise() * DLossDActivated.array();
						});

					//mDLossDWeight = (mDActivatedDValue * ((*layer)->inputs()).transpose() /* DValue/DWeight */)/* DActivated/DWeight */.array().colwise() * mDLossDActivated.array();

					if (layer != std::prev(network->layers().rend(), 2)) {
						// DActivated/DInput
						auto WeightsArray = (*layer)->weights().array();
						mDActivatedDInput.resize(mDActivatedDValue.cols());
						std::transform(std::execution::par_unseq, mDActivatedDValue.colwise().begin(), mDActivatedDValue.colwise().end(), mDActivatedDInput.begin(), [&](const auto& DActivatedDValue) {
							return WeightsArray.colwise() * DActivatedDValue.array();
							});
						//mDActivatedDInput = (*layer)->weights().colwise() /* DValue/DInput */ * mDActivatedDValue.array();
						// DLoss/DInput
						//auto mDLossDInputColwise = mDLossDInputColwise;
						mDLossDInput.noalias() = Eigen::MatrixX<TValue>((*layer)->weights().cols(), mDActivatedDInput.size());
						std::transform(std::execution::par_unseq, mDActivatedDInput.begin(), mDActivatedDInput.end(), mDLossDActivated.colwise().begin(), mDLossDInput.colwise().begin(), [](const auto& DActivatedDInput, const auto& DLossDActivated) {
							return DActivatedDInput.transpose() * DLossDActivated;
							});

						//SOREN_CORE_INFO(mDLossDInput);
						//mDLossDInput.noalias() = (mDActivatedDInput.transpose() * mDLossDActivated);
					}

					// Change Weights
					//SOREN_CORE_INFO(mDLossDWeight.front());
					//SOREN_CORE_INFO(mDLossDWeight.back());
					Eigen::MatrixX<TValue> zeroInit = Eigen::MatrixX<TValue>::Zero(mDLossDWeight.front().rows(), mDLossDWeight.front().cols());

					Eigen::MatrixX<TValue> sum = std::accumulate(mDLossDWeight.begin(), mDLossDWeight.end(), zeroInit);
					//SOREN_CORE_INFO(sum);
					//SOREN_CORE_INFO(sum / mDLossDWeight.size());
					(*layer)->weights() -= learning_rate * (sum / mDLossDWeight.size());
				}
			}
		}

		template<typename TValue, typename TBias = TValue, typename TWeight = TValue>
		class OptimizerBase {
		public:
			virtual ~OptimizerBase() {};
			virtual void operator ()(std::shared_ptr<Neural::Network<TValue, TBias, TWeight>>& network, const Eigen::Ref<const Eigen::MatrixX<TValue>>& DLossDLayerActivation, size_t epoch) {
				network;
				DLossDLayerActivation;
				epoch;
				SOREN_CORE_WARN("OptimizerBase {0}", __FUNCTION__);
				//return prediction;
			};

		};

		template<typename TValue, typename TBias = TValue, typename TWeight = TValue, typename ...Args>
		using OptimizerFunction = std::function<void(std::shared_ptr<Neural::Network<TValue, TBias, TWeight>>& network, const Eigen::Ref<const Eigen::MatrixX<TValue>>& DLossDLayerActivation, size_t epoch, Args...)>;

		template<typename TValue, typename TBias = TValue, typename TWeight = TValue, typename ...Args>
		class Optimizer : public OptimizerBase<TValue, TBias, TWeight> {
		public:

			Optimizer(
				OptimizerFunction<TValue, TBias, TWeight, Args...> function = Optimizer::Default,
				Args... args)
				: mOptimizerFunction(std::forward<OptimizerFunction<TValue, TBias, TWeight, Args...>>(function)), mArgs(std::forward<Args>(args)...)
			{}

			static void Default(std::shared_ptr<Neural::Network<TValue, TBias, TWeight>>& network, const Eigen::Ref<const Eigen::MatrixX<TValue>>& DLossDLayerActivation, size_t epoch) {
				network;
				DLossDLayerActivation;
				epoch;
				SOREN_CORE_WARN("Optimizer {0}", __FUNCTION__);
				return;
			}

			void operator ()(std::shared_ptr<Neural::Network<TValue, TBias, TWeight>>& network, const Eigen::Ref<const Eigen::MatrixX<TValue>>& DLossDLayerActivation, size_t epoch) override {
				return std::apply(mOptimizerFunction, std::make_tuple(std::ref(network), std::cref(DLossDLayerActivation), epoch, std::get<Args>(std::forward<std::tuple<Args...>>(mArgs))...));
			}

			static Optimizer<TValue, TBias, TWeight, double> GradientDescent() {
				return Optimizer<TValue, TBias, TWeight, double>(Function::GradientDescent<TValue, TBias, TWeight>, 0.01);
			}

		private:
			OptimizerFunction<TValue, TBias, TWeight, Args...> mOptimizerFunction;
			std::tuple<Args...> mArgs;
		};

		//template<typename TValue, typename TBias = TValue, typename TWeight = TValue>
		//static Optimizer<TValue, TBias, TWeight> GradientDescent{ Optimizer<TValue, TBias, TWeight>::GradientDescent() };

		template<typename TValue, typename TBias = TValue, typename TWeight = TValue>
		class GradientDescent : public Optimizer<TValue, TBias, TWeight> {
		public:

			struct LayerData {
				Eigen::MatrixX<TValue> DActivatedDValue;
				Eigen::MatrixX<TValue> DLossDActivated;
				Eigen::MatrixX<TValue> DLossDInput;

				std::vector<Eigen::MatrixX<TValue>> DActivatedDWeight;
				std::vector<Eigen::MatrixX<TValue>> DLossDWeight;
				std::vector<Eigen::MatrixX<TValue>> DLossDWeightMomentum;
				std::vector<Eigen::MatrixX<TValue>> DActivatedDInput;
				std::vector<std::shared_ptr<Eigen::MatrixX<TValue>>> DLayerActivationDActivated;

				Eigen::MatrixX<TValue> mZeroInit;
			};

			GradientDescent(
				double learning_rate = 0.01,
				double momentum = 0.0)
				: Optimizer<TValue, TBias, TWeight>(),
				  mLearningRate(learning_rate),
				  mMomentum(momentum)
			{}
			void operator ()(std::shared_ptr<Neural::Network<TValue, TBias, TWeight>>& network, const Eigen::Ref<const Eigen::MatrixX<TValue>>& DLossDLayerActivation, size_t epoch) override
			{
				epoch;
				if (!mInitialized) {
					mLayerData.resize(network->layers().size());

					auto iLayerData = mLayerData.begin();
					auto iInputLayer = std::prev(network->layers().rend(), 1);

					(*iLayerData).DLossDInput.noalias() = DLossDLayerActivation;

					for (auto layer = network->layers().rbegin(); layer != iInputLayer; layer++, iLayerData++) {
						size_t weightedCols = (*layer)->weighted().cols();
						size_t weightsCols = (*layer)->weights().cols();
						size_t weightsRows = (*layer)->weights().rows();

						auto& rDLossDInput = (*iLayerData).DLossDInput;

						// DLossDActivated
						auto& rDLossDActivated = (*iLayerData).DLossDActivated;
						rDLossDActivated.resize(rDLossDInput.rows(), rDLossDInput.cols());

						// DActivated/DWeight
						auto& rDActivatedDWeight = (*iLayerData).DActivatedDWeight;
						rDActivatedDWeight.resize(weightedCols);

						// DLoss/DWeight
						auto& rDLossDWeight = (*iLayerData).DLossDWeight;
						rDLossDWeight.resize(weightedCols);

						if (layer != std::prev(network->layers().rend(), 2)) {
							// DActivated/DInput
							auto& rDActivatedDInput = (*iLayerData).DActivatedDInput;
							rDActivatedDInput.resize(weightedCols);

							// DLoss/DInput
							auto& rDLossDInputNext = (*(std::next(iLayerData, 1))).DLossDInput;
							rDLossDInputNext.resize((*layer)->weights().cols(), weightedCols);
						}

						// ZeroInit
						auto& rZeroInit = (*iLayerData).mZeroInit;
						rZeroInit.noalias() = Eigen::MatrixX<TValue>::Zero(weightsRows, weightsCols);
					}
					// Set the sizes for the data
					mInitialized = true;
				}

				auto iLayerData = mLayerData.begin();
				auto iInputLayer = std::prev(network->layers().rend(), 1);

				(*iLayerData).DLossDInput.noalias() = DLossDLayerActivation;


				// Reasigning Gradients is causing memory to be released and created evey call

				for (auto layer = network->layers().rbegin(); layer != iInputLayer; layer++, iLayerData++) {
					auto& rDLossDInput = (*iLayerData).DLossDInput;
					// DLayerActivation/DActivated
					auto& rDLayerActivationDActivated = (*iLayerData).DLayerActivationDActivated;
					rDLayerActivationDActivated = ((*layer)->layerActivation())->prime((*layer)->activated());
					auto& rDLossDActivated = (*iLayerData).DLossDActivated;
					//rDLossDActivated.resize(rDLossDInput.rows(), rDLossDInput.cols());

					// DLossDActivated
					std::transform(std::execution::par_unseq, rDLayerActivationDActivated.begin(), rDLayerActivationDActivated.end(), rDLossDInput.colwise().begin(), rDLossDActivated.colwise().begin(), [](const auto& DLayerActivationDActivated, const auto& DLossDInput) {
						//SOREN_CORE_INFO(*DLayerActivationDActivated);
						return (*DLayerActivationDActivated) * DLossDInput;
						});

					// DActivated/DValue
					auto& rDActivatedDValue = (*iLayerData).DActivatedDValue;
					rDActivatedDValue = ((*layer)->weighted().array().colwise() + (*layer)->biases().array()).unaryExpr([&](auto val) { return ((*layer)->activation())->prime(val); });

					// DActivated/DWeight
					auto& rDActivatedDWeight = (*iLayerData).DActivatedDWeight;
					//rDActivatedDWeight.resize(rDActivatedDValue.cols());
					std::transform(std::execution::par_unseq, rDActivatedDValue.colwise().begin(), rDActivatedDValue.colwise().end(), (*layer)->inputs().colwise().begin(), rDActivatedDWeight.begin(), [](const auto& mDActivatedDValue, const auto& inputs) {
						return mDActivatedDValue * inputs.transpose();
						});

					// DLoss/DWeight
					auto& rDLossDWeight = (*iLayerData).DLossDWeight;
					//rDLossDWeight.resize(rDActivatedDWeight.size());
					std::transform(std::execution::par_unseq, rDActivatedDWeight.begin(), rDActivatedDWeight.end(), rDLossDActivated.colwise().begin(), rDLossDWeight.begin(), [](const auto& DActivatedDWeight, const auto& DLossDActivated) {
						return DActivatedDWeight.array().colwise() * DLossDActivated.array();
						});

					if (mMomentum != 0) {
						// DLoss/DWeight (Momentum)
						auto& rDLossDWeightMomentum = (*iLayerData).DLossDWeightMomentum;
						std::transform(std::execution::par_unseq, rDLossDWeightMomentum.begin(), rDLossDWeightMomentum.end(), rDLossDWeight.begin(), rDLossDWeightMomentum.begin(), [&](const auto& DLossDWeightMomentum, const auto& DLossDWeight) {
							return (mMomentum * DLossDWeightMomentum) + ((1 - mMomentum) * DLossDWeight);
							});
					}


					if (layer != std::prev(network->layers().rend(), 2)) {
						// DActivated/DInput
						auto& rDActivatedDInput = (*iLayerData).DActivatedDInput;
						//rDActivatedDInput.resize(rDActivatedDValue.cols());
						std::transform(std::execution::par_unseq, rDActivatedDValue.colwise().begin(), rDActivatedDValue.colwise().end(), rDActivatedDInput.begin(), [&](const auto& DActivatedDValue) {
							return (*layer)->weights().array().colwise() * DActivatedDValue.array();
							});

						// DLoss/DInput
						auto& rDLossDInputNext = (*(std::next(iLayerData, 1))).DLossDInput;
						//rDLossDInputNext.resize((*layer)->weights().cols(), rDActivatedDInput.size());
						std::transform(std::execution::par_unseq, rDActivatedDInput.begin(), rDActivatedDInput.end(), rDLossDActivated.colwise().begin(), rDLossDInputNext.colwise().begin(), [](const auto& DActivatedDInput, const auto& DLossDActivated) {
							return DActivatedDInput.transpose() * DLossDActivated;
							});
					}

					// Change Weights
					auto& rZeroInit = (*iLayerData).mZeroInit;
					auto& rDLossDWeightSum = mMomentum != 0 ? (*iLayerData).DLossDWeightMomentum : (*iLayerData).DLossDWeight;

					Eigen::MatrixX<TValue> sum = std::reduce(std::execution::par_unseq, rDLossDWeightSum.begin(), rDLossDWeightSum.end(), rZeroInit);
					(*layer)->weights() -= mLearningRate * (sum / rDLossDWeightSum.size());
				}
			}
		private:
			//Eigen::MatrixX<TValue> mZeroInit;

			std::vector<LayerData> mLayerData;

			//Eigen::MatrixX<TValue> mDActivatedDValue;
			//Eigen::MatrixX<TValue> mDActivatedDInput;
			//Eigen::MatrixX<TValue> mDLossDWeight;
			//Eigen::MatrixX<TValue> mDLossDActivated;
			//Eigen::MatrixX<TValue> mDLossDInput;

			//std::vector<Eigen::MatrixX<TValue>> mDActivatedDWeight;
			//std::vector<Eigen::MatrixX<TValue>> mDLossDWeight;
			//std::vector<Eigen::MatrixX<TValue>> mDActivatedDInput;
			//std::vector<std::shared_ptr<Eigen::MatrixX<TValue>>> DLossDWeight;
			//std::vector<std::shared_ptr<Eigen::MatrixX<TValue>>> mDLayerActivationDActivated;

			double mLearningRate;
			double mMomentum;

			bool mInitialized{ false };
		};

		template<typename TValue, typename TBias = TValue, typename TWeight = TValue>
		class Adam : public Optimizer<TValue, TBias, TWeight> {
		public:
			Adam(double learning_rate = 0.001,
				double beta_1 = 0.9,
				double beta_2 = 0.999,
				double epsilon = 1e-07)
				: Optimizer<TValue, TBias, TWeight>(),
				mLearningRate(learning_rate),
				mBeta1(beta_1),
				mBeta2(beta_2),
				mEpsilon(epsilon)
			{}
		private:
			double mLearningRate;
			double mBeta1;
			double mBeta2;
			double mEpsilon;
		};

		template<typename TValue, typename TBias = TValue, typename TWeight = TValue>
		class RMSProp : public Optimizer<TValue, TBias, TWeight> {
		public:
			RMSProp(
				double learning_rate = 0.001,
				double rho = 0.9,
				double momentum = 0.0,
				double epsilon = 1e-07)
				: Optimizer<TValue, TBias, TWeight>(),
				  mLearningRate(learning_rate),
				  mRho(rho),
				  mMomentum(momentum),
				  mEpsilon(epsilon)
			{}
		private:
			double mLearningRate;
			double mRho;
			double mMomentum;
			double mEpsilon;
		};

	} // namespace Optimizer
} // namespace Soren