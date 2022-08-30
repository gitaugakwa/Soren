#pragma once

// caused warning C4003: not enough arguments for function-like macro invocation 'min'
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include "Core.h"
#include "Base.h"

#include "Status/Status.h"

#include "Log/Log.h"
#include "Optimizer/Optimizer.h"
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

		struct Result {
			size_t index;
			double loss;
		};

		struct BatchResult {
			double cost;
			double accuracy;
			double precision;
			double recal;
		};

		template<typename TValue = double, typename TTarget = TValue, typename TBias = TValue, typename TWeight = TValue>
		class SOREN_API Model {
		public:

			Model(std::shared_ptr<Network<TValue, TBias, TWeight>> network)
				: mNetwork(network) {
			}

			Model(std::shared_ptr<Network<TValue, TBias, TWeight>> network, std::shared_ptr<Loss::Vector::LossBase<TValue, TTarget>> loss)
				: mNetwork(network), mLoss(loss) {
			}

			template<typename TOutput = TTarget>
			struct DataPair {
				Eigen::MatrixX<TValue> input;
				Eigen::MatrixX<TOutput> output;
			};

			template<typename TOutput = TTarget>
			struct PassThroughResult {
				Eigen::VectorX<double> loss;
				Eigen::VectorX<double> accuracy;
				Eigen::MatrixX<TOutput> output;
				Eigen::MatrixX<TValue> DLossDOutput;
			};
			template<typename TOutput = TTarget>
			struct ForwardPropagationResult {
				//size_t prediction{ 0 };
				Eigen::MatrixX<TOutput> output;
			};
			template<typename TOutput = TTarget>
			struct EpochResult {
				double loss{ 0 };
				double accuracy{ 0 };
			};


			//inline void scale(double scale) { mScale = scale; }
			//inline void loss(std::shared_ptr<Loss::Loss<TValue, TTarget>> loss) { mLoss = loss; }

			inline void setLoss(std::shared_ptr<Loss::Loss<TValue, TTarget>> loss) { mLoss = loss; }

			inline const BatchResult trainResult() const { return mTrainResult; }
			inline const BatchResult testResult() const { return mTestResult; }

			inline void trainReset() { mTrainResult = { 0,0,0,0 }; backPropTime = {}; };
			inline void testReset() { mTestResult = { 0,0,0,0 }; };

			BatchResult train(
				std::function<DataPair<>(size_t index)> generator,
				size_t dataSize,
				size_t miniBatchSize,
				size_t epochs = 5,
#ifdef SOREN_DEBUG
				size_t interval = 100
#else
				size_t interval = 1000
#endif // SOREN_DEBUG
			) {
				Timer::Stopwatch stopwatch;
				
				for (size_t iEpoch = 1; iEpoch < epochs + 1; iEpoch++) {
					EpochResult<> epochResult = epoch(iEpoch, generator, dataSize, miniBatchSize, interval);
					mTrainResult.accuracy += epochResult.accuracy;
					mTrainResult.cost += epochResult.loss;
				}
				mTrainResult.accuracy /= epochs;
				mTrainResult.cost /= epochs;

				SOREN_CORE_INFO("[TRAIN] [{0}] - Cost: {1:.4f} - Accuracy: {2:.4f} | {3}ms", epochs, mTrainResult.cost, mTrainResult.accuracy, stopwatch.Lap().milliseconds());

				return mTrainResult;
			}

			EpochResult<> epoch(
				size_t epoch,
				std::function<DataPair<>(size_t index)> generator,
				size_t dataSize,
				size_t miniBatchSize,
				size_t interval
			) {
				Timer::Stopwatch trainStopwatch;
				Timer::Stopwatch stopwatch;
				TimeSlice trainDuration;

				EpochResult<> epochData{};

				for (size_t iData = 0; iData < dataSize; iData++) {
					DataPair<> dataPair = generator(iData);
					PassThroughResult<> result = trainPassThrough<TTarget>(dataPair.input, dataPair.output);

					epochData.accuracy += result.accuracy.mean();
					epochData.loss += result.loss.mean();

					mOptimizer->operator()(mNetwork, result.DLossDOutput, epoch);
					//mNetwork->backPropagate(batchData.DLossDOutput * learningRate);

					if (iData != 0 && (iData % interval) == 0) {
						SOREN_CORE_INFO("[EPOCH] [{0}] ({1}|{2}) - Cost: {3:.4f} - Accuracy: {4:.4f} | {5}ms", epoch, iData, miniBatchSize, epochData.loss / iData, (double)epochData.accuracy / iData, stopwatch.Lap().milliseconds());
					}
				}
				epochData.loss /= dataSize;
				epochData.accuracy /= dataSize;

				SOREN_CORE_INFO("[EPOCH] [{0}] ({1}) - Cost: {3:.4f} - Accuracy: {4:.4f} | {5}ms", epoch, (dataSize * miniBatchSize) - 1, miniBatchSize, epochData.loss, epochData.accuracy, stopwatch.Lap().milliseconds());
				return epochData;
			}

			ForwardPropagationResult<> forwardPropagation(const Eigen::Ref<const Eigen::MatrixX<TValue>>& input) {
				Eigen::MatrixX<TValue> output{ mNetwork->input(input).resolve().output() };
				//size_t prediction = std::distance(output.begin(), std::max_element(output.begin(), output.end()));
				return { output };
			}

			template<typename TTargetOutput = TTarget>
			Eigen::MatrixX<TValue> gradient(const Eigen::Ref<const Eigen::MatrixX<TValue>>& output, const Eigen::Ref<const Eigen::MatrixX<TTargetOutput>>& targetOutput) {
				// Gradient Resolution currently mean
				// DLoss/DOutput
				return (mLoss)->prime(output, targetOutput);
			}

			template<typename TTargetOutput = TTarget>
			PassThroughResult<> trainPassThrough(const Eigen::Ref<const Eigen::MatrixX<TValue>>& input, const Eigen::Ref<const Eigen::MatrixX<TTargetOutput>>& targetOutput) {
				auto [ output ] = forwardPropagation(input);
				//SOREN_CORE_INFO((mLoss)->operator()(output, targetOutput));
				//SOREN_CORE_INFO((mLoss)->operator()(output, targetOutput).colwise().sum());
				Eigen::VectorX<double> loss{ (mLoss)->operator()(output, targetOutput).colwise().sum().cast<double>() };
				//SOREN_CORE_INFO(output);
				//SOREN_CORE_INFO((mLoss)->operator()(output, targetOutput));
				//SOREN_CORE_INFO(loss);
				Eigen::VectorX<double> accuracy(output.cols());

				auto outputColwise = output.colwise();
				auto targetOutputColwise = targetOutput.colwise();

				auto outputColwisePointer = outputColwise.begin();
				auto targetOutputColwisePointer = targetOutputColwise.begin();
				for (size_t iCol = 0; outputColwisePointer != outputColwise.end(); outputColwisePointer++, targetOutputColwisePointer++, iCol++) {
					size_t targetLabel = (size_t)std::distance(targetOutputColwisePointer->begin(), std::max_element(targetOutputColwisePointer->begin(), targetOutputColwisePointer->end()));
					size_t outputLabel = (size_t)std::distance(outputColwisePointer->begin(), std::max_element(outputColwisePointer->begin(), outputColwisePointer->end()));
					accuracy(iCol) = targetLabel == outputLabel ? 1 : 0;
				}

				auto grad = gradient(output, targetOutput);
				//SOREN_CORE_INFO(target);
				//SOREN_CORE_INFO(output);
				//SOREN_CORE_INFO(grad);
				return { loss, accuracy, output, grad };
			}

			Timer::Stopwatch backPropStopwatch{};
			TimeSlice backPropTime{};

			BatchResult test(
				std::function<DataPair<>(size_t index)> generator,
				size_t dataSize,
#ifdef SOREN_DEBUG
				size_t interval = 100
#else
				size_t interval = 1000
#endif // SOREN_DEBUG
			) {
				Timer::Stopwatch stopwatch;

				for (size_t iData = 0; iData < dataSize; iData++) {
					DataPair<> dataPair = generator(iData);
					PassThroughResult<> result = testPassThrough<TTarget>(dataPair.input, dataPair.output);
					mTestResult.accuracy += result.accuracy.mean();
					mTestResult.cost += result.loss.mean();

					if (iData != 0 && (iData % interval) == 0) {
						SOREN_CORE_INFO("[TEST] ({0}) - Cost: {1:.4f} - Accuracy: {2:.4f} | {3}ms", iData, mTestResult.cost / iData, (double)mTestResult.accuracy / iData, stopwatch.Lap().milliseconds());
					}
					// For Precision and Recal
					// They need to be calculated for 
				}

				mTestResult.cost /= dataSize;
				mTestResult.accuracy /= dataSize;

				SOREN_CORE_INFO("[TEST] ({0}) - Cost: {1:.4f} - Accuracy: {2:.4f} | {3}ms", dataSize - 1, mTestResult.cost, mTestResult.accuracy, stopwatch.Lap().milliseconds());
				return mTestResult;
			}


			template<typename TTargetOutput = TTarget>
			PassThroughResult<> testPassThrough(const Eigen::Ref<const Eigen::MatrixX<TValue>>& input, const Eigen::Ref<const Eigen::MatrixX<TTargetOutput>>& targetOutput) {
				auto [output] = forwardPropagation(input);
				Eigen::VectorX<double> loss{ (mLoss)->operator()(output, targetOutput).colwise().sum().cast<double>() };
				Eigen::VectorX<double> accuracy(output.cols());

				auto outputColwise = output.colwise();
				auto targetOutputColwise = targetOutput.colwise();

				auto outputColwisePointer = outputColwise.begin();
				auto targetOutputColwisePointer = targetOutputColwise.begin();
				for (size_t iCol = 0; outputColwisePointer != outputColwise.end(); outputColwisePointer++, targetOutputColwisePointer++, iCol++) {
					size_t targetLabel = (size_t)std::distance(targetOutputColwisePointer->begin(), std::max_element(targetOutputColwisePointer->begin(), targetOutputColwisePointer->end()));
					size_t outputLabel = (size_t)std::distance(outputColwisePointer->begin(), std::max_element(outputColwisePointer->begin(), outputColwisePointer->end()));
					accuracy(iCol) = targetLabel == outputLabel ? 1 : 0;
				}
				
				return { loss, accuracy, output };
			}

			template<typename TTargetOutput = TTarget>
			Result test(std::vector<TValue>& input, std::vector<TTargetOutput>& targetOutput) {
				// Get neural network prediction
				Eigen::Vector<TValue, Eigen::Dynamic> output = mNetwork->input(input).resolve().output();

				// SoftMax
				Eigen::Vector<TValue, Eigen::Dynamic> softMax = Activation::Function::Vector::SoftMax<TValue, TValue>(output);

				// Result
				size_t index = std::distance(softMax.data(), std::max_element(softMax.data(), softMax.data() + softMax.rows()));

				// Loss
				Eigen::Vector<TValue, Eigen::Dynamic> lossVec = (mLoss)->operator()(softMax, targetOutput);
				double loss{ std::accumulate(lossVec.begin(), lossVec.end(), (double)0) };

				return { index, loss };
			}

		private:

			std::shared_ptr<Optimizer::OptimizerBase<TValue, TBias, TWeight>> mOptimizer{ std::make_shared<Soren::Optimizer::GradientDescent<TValue, TBias, TWeight>>(Optimizer::GradientDescent<TValue, TBias, TWeight>()) };
			std::shared_ptr<Loss::Vector::LossBase<TValue, TTarget>> mLoss{ std::make_shared<Soren::Loss::Loss<TValue, TTarget>>(Loss::CategoricalCrossentropy<TValue, TTarget>) };
			std::shared_ptr<Network<TValue, TBias, TWeight>> mNetwork;
			BatchResult mTrainResult{};
			BatchResult mTestResult{};
		};

	} // namespace Model
} // namespace Soren
