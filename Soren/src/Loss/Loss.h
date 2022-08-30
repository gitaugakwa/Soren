#pragma once
#include "Log/Log.h"
#include <math.h>

#include "unsupported/Eigen/MatrixFunctions"


// BinaryCrossentropy
// BinaryFocalCrossentropy
// CategoricalCrossentropy
// CategoricalHinge
// CosineSimilarity
// Hinge
// Huber
// KLDivergence
// LogCosh
// MeanAbsoluteError
// MeanAbsolutePercentageError
// MeanSquaredError
// Poisson
// Reduction
// SparseCategoricalCrossentropy
// SquaredHinge



namespace Soren {

	namespace Loss {

		namespace Function {

			enum class LossFunction
			{
				BinaryCrossentropy, BinaryFocalCrossentropy, CategoricalCrossentropy, SparseCategoricalCrossentropy,
				Hinge, CategoricalHinge, SquaredHinge,
				CosineSimilarity,
				Huber,
				KLDivergence,
				LogCosh,
				MeanAbsoluteError, MeanAbsolutePercentageError, MeanSquaredError,
				Poisson,
				Reduction
			};

			// Default will be without logits
			// So one is meant to convert their logits into probabilities
			// The classes will have that functionality

			// BinaryCrossentropy
			template <typename TPrediction>
			static constexpr Eigen::VectorX<TPrediction> BinaryCrossentropy(const Eigen::VectorX<TPrediction>& prediction, const std::vector<bool>& target)
			{
				size_t size = prediction.rows();
				if (size != target.size()) {
					throw std::invalid_argument("Target and prediction are not of the same size");
				}

				if (std::accumulate(target.begin(), target.end(), (TPrediction)0) != (TPrediction)1) {
					throw std::invalid_argument("Target total is not between 0 and 1");
				}

				return prediction.binaryExpr(
					Eigen::Map<Eigen::VectorX<TPrediction>>(std::vector<TPrediction>(target.begin(), target.end()).data(), size),
					[](TPrediction prediction, TPrediction target) -> TPrediction {
						if (target == 1) {
							return -(std::log(prediction));
						}
						return -(std::log(1 - prediction));
					});
			}

			// BinaryFocalCrossentropy
			template <typename TPrediction, typename TGamma, std::enable_if_t<std::is_integral<TGamma>::value || std::is_floating_point<TGamma>::value, int> = 0>
			static constexpr Eigen::VectorX<TPrediction> BinaryFocalCrossentropy(const Eigen::VectorX<TPrediction>& prediction, const std::vector<bool>& target, TGamma gamma = 2)
			{
				size_t size = prediction.rows();
				if (size != target.size()) {
					throw std::invalid_argument("Target and prediction are not of the same size");
				}

				if (std::accumulate(target.begin(), target.end(), (TPrediction)0) != (TPrediction)1) {
					throw std::invalid_argument("Target total is not between 0 and 1");
				}

				return prediction.binaryExpr(
					Eigen::Map<Eigen::VectorX<TPrediction>>(std::vector<TPrediction>(target.begin(), target.end()).data(), size),
					[](TPrediction prediction, TPrediction target) -> TPrediction {
						if (target == 1) {
							return -(std::pow((1 - prediction), gamma) * std::log(prediction));
						}
						return -(std::pow((prediction), gamma) * std::log(1 - prediction));
					});
			}

			// CategoricalCrossentropy
			template <typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
			static constexpr Eigen::MatrixX<TPrediction> CategoricalCrossentropy(const Eigen::Ref<const Eigen::MatrixX<TPrediction>>& prediction, const Eigen::Ref<const Eigen::MatrixX<TTarget>>& target)
			{
#ifdef SOREN_DEBUG
				size_t cols = prediction.cols();
				if (cols != (size_t)target.cols()) {
					throw std::invalid_argument("Target and prediction are not of the same size");
				}
				auto predictionColwise = prediction.colwise();
				auto targetColwise = target.colwise();
				auto ptrPredictionColwise = predictionColwise.begin();
				auto ptrTargetColwise = targetColwise.begin();

				for (; ptrPredictionColwise != predictionColwise.end(); ptrPredictionColwise++, ptrTargetColwise++) {
					size_t size = ptrPredictionColwise->rows();
					if (size != (size_t)ptrTargetColwise->rows()) {
						throw std::invalid_argument("Target and prediction are not of the same size");
					}

					if (std::accumulate(ptrTargetColwise->begin(), ptrTargetColwise->end(), (TTarget)0) != (TTarget)1) {
						throw std::invalid_argument("Target total is not between 0 and 1");
					}
				}
#endif // SOREN_DEBUG


				#pragma warning( disable : 4305 ) // Disable Warning C4305: 'initializing': truncation from 'long double' to 'const RealScalar'
				return -(prediction.array().log() * target.cast<TPrediction>().array());
				#pragma warning( default : 4305 ) // Reenable Warning C4305: 'initializing': truncation from 'long double' to 'const RealScalar'

				/*return prediction.binaryExpr(
					,
					[](TPrediction val, TPrediction targetVal) -> TPrediction {
						return -(targetVal * std::log(val));
					});*/
			}

			// SparseCategoricalCrossentropy
			template <typename TPrediction>
			static constexpr Eigen::VectorX<TPrediction> SparseCategoricalCrossentropy(const Eigen::VectorX<TPrediction>& prediction, const std::vector<size_t>& indexVec)
			{
				size_t size = prediction.rows();
				if (indexVec.size() != 1) {
					throw std::invalid_argument("Index is not of size 1");
				}
				size_t index = indexVec[0];
				if (size - 1 <= index) {
					throw std::invalid_argument("Index is not in Prediction");
				}

				Eigen::VectorX<TPrediction> result = Eigen::VectorX<TPrediction>::Zero(size);
				//result.setZero();

				result(index) = -(std::log(prediction[index]));

				return result;
			}

			namespace Derivative {

				enum class LossFunctionDerivative
				{
					BinaryCrossentropy, BinaryFocalCrossentropy, CategoricalCrossentropy, SparseCategoricalCrossentropy,
					Hinge, CategoricalHinge, SquaredHinge,
					CosineSimilarity,
					Huber,
					KLDivergence,
					LogCosh,
					MeanAbsoluteError, MeanAbsolutePercentageError, MeanSquaredError,
					Poisson,
					Reduction
				};

				// BinaryCrossentropy
				template <typename TPrediction>
				static constexpr Eigen::VectorX<TPrediction> BinaryCrossentropy(const Eigen::VectorX<TPrediction>& prediction, const std::vector<bool>& target)
				{
					size_t size = prediction.rows();
					if (size != target.size()) {
						throw std::invalid_argument("Target and prediction are not of the same size");
					}

					if (std::accumulate(target.begin(), target.end(), (TPrediction)0) != (TPrediction)1) {
						throw std::invalid_argument("Target total is not between 0 and 1");
					}

					return prediction.binaryExpr(
						Eigen::Map<Eigen::VectorX<TPrediction>>(std::vector<TPrediction>(target.begin(), target.end()).data(), size),
						[](TPrediction prediction, TPrediction target) -> TPrediction {
							return -((prediction - target) / (prediction * (prediction - 1)));
						});
				}

				// BinaryFocalCrossentropy
				template <typename TPrediction, typename TGamma, std::enable_if_t<std::is_integral<TGamma>::value || std::is_floating_point<TGamma>::value, int> = 0>
				static constexpr Eigen::VectorX<TPrediction> BinaryFocalCrossentropy(const Eigen::VectorX<TPrediction>& prediction, const std::vector<bool>& target, TGamma gamma = 2)
				{
					size_t size = prediction.rows();
					if (size != target.size()) {
						throw std::invalid_argument("Target and prediction are not of the same size");
					}

					if (std::accumulate(target.begin(), target.end(), (TPrediction)0) != (TPrediction)1) {
						throw std::invalid_argument("Target total is not between 0 and 1");
					}

					return prediction.binaryExpr(
						Eigen::Map<Eigen::VectorX<TPrediction>>(std::vector<TPrediction>(target.begin(), target.end()).data(), size),
						[](TPrediction prediction, TPrediction target) -> TPrediction {
							if (gamma == 0) {
								return -((prediction - target) / (prediction * (prediction - 1)));
							}
							if (target == 1) {
								return ((gamma * target) * (std::pow((1 - prediction), gamma - 1)) * std::log(prediction)) + (((1 - target) * std::pow(prediction, gamma)) / (1 - prediction)) - (gamma * (1 - prediction) * std::log(1 - prediction) * std::pow(prediction, (gamma - 1))) - ((target * std::pow((1 - prediction), gamma)) / prediction);
							}
							return ((std::pow(prediction, gamma) / (1 - prediction)) - (gamma * std::log(1 - prediction) * std::pow(prediction, gamma - 1)));
						});
				}

				// CategoricalCrossentropy
				template <typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
				static constexpr Eigen::MatrixX<TPrediction> CategoricalCrossentropy(const Eigen::Ref<const Eigen::MatrixX<TPrediction>>& prediction, const Eigen::Ref<const Eigen::MatrixX<TTarget>>& target)
				{
#ifdef SOREN_DEBUG
					size_t cols = prediction.cols();
					if (cols != (size_t)target.cols()) {
						throw std::invalid_argument("Target and prediction are not of the same size");
					}
					auto predictionColwise = prediction.colwise();
					auto targetColwise = target.colwise();
					auto ptrPredictionColwise = predictionColwise.begin();
					auto ptrTargetColwise = targetColwise.begin();
					for (; ptrPredictionColwise != predictionColwise.end(); ptrPredictionColwise++, ptrTargetColwise++) {
						size_t size = ptrPredictionColwise->rows();
						if (size != (size_t)ptrTargetColwise->rows()) {
							throw std::invalid_argument("Target and prediction are not of the same size");
						}

						if (std::accumulate(ptrTargetColwise->begin(), ptrTargetColwise->end(), (TTarget)0) != (TTarget)1) {
							throw std::invalid_argument("Target total is not between 0 and 1");
						}
					}
#endif // SOREN_DEBUG

					return -(target.cast<TPrediction>().array() / prediction.array());

					//return -(Eigen::Map<Eigen::VectorX<TPrediction>>(std::vector<TPrediction>(target.begin(), target.end()).data(), size).array().colwise() / prediction.array());


					/*return prediction.binaryExpr(
						Eigen::Map<Eigen::VectorX<TPrediction>>(std::vector<TPrediction>(target.begin(), target.end()).data(), size),
						[](TPrediction val, TPrediction targetVal) -> TPrediction {
							return -(targetVal / val);
						});*/
				}

				// SparseCategoricalCrossentropy
				template <typename TPrediction>
				static constexpr Eigen::VectorX<TPrediction> SparseCategoricalCrossentropy(const Eigen::VectorX<TPrediction>& prediction, const std::vector<size_t>& indexVec)
				{
					size_t size = prediction.rows();
					if (indexVec.size() != 1) {
						throw std::invalid_argument("Index is not of size 1");
					}
					size_t index = indexVec[0];
					if (size - 1 <= index) {
						throw std::invalid_argument("Index is not in Prediction");
					}

					Eigen::VectorX<TPrediction> result = Eigen::VectorX<TPrediction>::Zero(size);

					result(index) = -(1 / prediction[index]);

					return result;
				}

				//// Hinge
				//template <typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
				//static constexpr Eigen::VectorX<TPrediction> Hinge(const Eigen::VectorX<TPrediction>& prediction, const std::vector<TTarget>& target)
				//{
				//	return prediction.binaryExpr(
				//		Eigen::Map<Eigen::VectorX<TPrediction>>(std::vector<TPrediction>(target.begin(), target.end()).data(), size),
				//		[](TPrediction val, TPrediction targetVal) -> TPrediction {
				//			return std::max(1 - (val * targetVal), 0);
				//		});
				//}

				//// CategoricalHinge
				//template <typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
				//static constexpr Eigen::VectorX<TPrediction> CategoricalHinge(const Eigen::VectorX<TPrediction>& prediction, const std::vector<TTarget>& target)
				//{
				//	size_t size = prediction.rows();
				//	if (size != target.size()) {
				//		throw std::invalid_argument("Target and prediction are not of the same size");
				//	}

				//	Eigen::VectorX<TTarget> targetVec = Eigen::Map<Eigen::VectorX<TTarget>>(target.data(), size);
				//	TPrediction pos = (targetVec.array().colwise() * prediction.array()).sum();

				//	TPrediction neg = ((Eigen::VectorX<TTarget>::Ones(size) - targetVec).array().colwise() * prediction.array()).maxCoeff();

				//	Eigen::VectorX<TPrediction> result = Eigen::VectorX<TPrediction>::Zero();
				//	result[0] = std::max((neg - pos) + 1, 0);

				//	return result;
				//}

			} // namespace Derivative
		} // namespace Function

		template<typename TPrediction, typename TPredictionOutput = TPrediction, typename TTarget= TPrediction, typename TPrimePrediction = TPrediction, typename TPrimePredictionOutput = TPredictionOutput, typename TPrimeTarget = TTarget>
		class LossBase {
		public:
			virtual ~LossBase() {}
			virtual TPredictionOutput operator ()(TPrediction prediction, TTarget target) const {
				target;
				SOREN_CORE_WARN("ActivationBase {0}", __FUNCTION__);
				return prediction;
			};
			virtual TPrimePredictionOutput prime(TPrimePrediction prediction, TTarget target) const {
				target;
				SOREN_CORE_WARN("ActivationBase {0}", __FUNCTION__);
				return prediction;
			};
		};

		template<typename TPrediction, typename TTarget = TPrediction, typename ...Args>
		using LossFunction = std::function<Eigen::MatrixX<TPrediction>(const Eigen::Ref<const Eigen::MatrixX<TPrediction>>&, const Eigen::Ref<const Eigen::MatrixX<TTarget>>&, Args...)>;

		namespace Vector {
			template<typename TPrediction, typename TTarget = TPrediction, typename TPrimePrediction = TPrediction, typename TPrimeTarget = TTarget>
			using LossBase = Loss::LossBase< const Eigen::Ref<const Eigen::MatrixX<TPrediction>>&, Eigen::MatrixX<TPrediction>, const Eigen::Ref<const Eigen::MatrixX<TTarget>>&, const Eigen::Ref<const Eigen::MatrixX<TPrimePrediction>>&, Eigen::MatrixX<TPrimePrediction>, const Eigen::Ref<const Eigen::MatrixX<TPrimeTarget>>& >;
		}

		template<typename TPrediction, typename TTarget = TPrediction, typename ...Args>
		class Loss : public LossBase< const Eigen::Ref<const Eigen::MatrixX<TPrediction>>&, Eigen::MatrixX<TPrediction>, const Eigen::Ref<const Eigen::MatrixX<TTarget>>& > {
		public:

			Loss(
				LossFunction<TPrediction, TTarget, Args...> function,
				std::function<Eigen::MatrixX<TPrediction>(const Eigen::Ref<const Eigen::MatrixX<TPrediction>>&, const Eigen::Ref<const Eigen::MatrixX<TTarget>>&, Args...)> derivative,
				std::function<Eigen::MatrixX<TPrediction>(const Eigen::Ref<const Eigen::MatrixX<TPrediction>>&)> probability = Activation::Function::Vector::Identity<TPrediction, TPrediction>,
				Args... args)
				: mLossFunction(function), mLossPrimeFunction(derivative), mProbabilityFunction(probability), mArgs(std::forward<Args>(args)...)
			{}

			Eigen::MatrixX<TPrediction> operator ()(const Eigen::Ref<const Eigen::MatrixX<TPrediction>>& prediction, const Eigen::Ref<const Eigen::MatrixX<TTarget>>& target) const override {
				auto diff = (prediction.colwise().sum().array().colwise() - Eigen::VectorX<TPrediction>::Ones(1).array()).array().abs().maxCoeff();
				if (diff > std::pow(1, -3)) {
					return std::apply(mLossFunction, std::make_tuple(mProbabilityFunction(prediction), std::cref(target), std::get<Args>(std::forward<std::tuple<Args...>>(mArgs))...));
					//return mLossFunction(mProbabilityFunction(prediction), target, std::forward<Args>(mArgs)...);
				}
				return std::apply(mLossFunction, std::make_tuple(std::cref(prediction), std::cref(target), std::get<Args>(std::forward<std::tuple<Args...>>(mArgs))...));
				//return mLossFunction(prediction, target, std::forward<Args>(mArgs)...);
			}

			Eigen::MatrixX<TPrediction> prime(const Eigen::Ref<const Eigen::MatrixX<TPrediction>>& prediction, const Eigen::Ref<const Eigen::MatrixX<TTarget>>& target) const override {
				auto diff = (prediction.colwise().sum().array().colwise() - Eigen::VectorX<TPrediction>::Ones(1).array()).array().abs().maxCoeff();
				//auto diff = std::abs(prediction.sum() - 1);
				if (diff > std::pow(1, -3)) {
					return std::apply(mLossPrimeFunction, std::make_tuple(mProbabilityFunction(prediction), std::cref(target), std::get<Args>(std::forward<std::tuple<Args...>>(mArgs))...));
					//return mLossPrimeFunction(mProbabilityFunction(prediction), target, std::forward<Args>(mArgs)...);
				}
				return std::apply(mLossPrimeFunction, std::make_tuple(std::cref(prediction), std::cref(target), std::get<Args>(std::forward<std::tuple<Args...>>(mArgs))...));
				//return mLossPrimeFunction(prediction, target, std::forward<Args>(mArgs)...);
			}

			//template<typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
			static Loss<TPrediction, bool, Args...> BinaryCrossentropy() {
				return Loss<TPrediction, bool, Args...>{Function::BinaryCrossentropy<TPrediction>, Function::Derivative::BinaryCrossentropy<TPrediction>, Activation::Function::Vector::Sigmoid<TPrediction>};
			}
			static Loss<TPrediction,bool, Args...> BinaryFocalCrossentropy() {
				return Loss<TPrediction, bool, Args...>(Function::BinaryFocalCrossentropy<TPrediction, Args...>, Function::Derivative::BinaryFocalCrossentropy<TPrediction, Args...>);
			}
			static Loss<TPrediction, TTarget, Args...> CategoricalCrossentropy() {
				return Loss<TPrediction, TTarget, Args...>(Function::CategoricalCrossentropy<TPrediction, TTarget>, Function::Derivative::CategoricalCrossentropy<TPrediction, TTarget>);//, Activation::Function::Vector::SoftMax<TPrediction>);
			}
			static Loss<TPrediction, size_t, Args...> SparseCategoricalCrossentropy() {
				return Loss<TPrediction, size_t, Args...>(Function::SparseCategoricalCrossentropy<TPrediction>, Function::Derivative::SparseCategoricalCrossentropy<TPrediction>, Activation::Function::Vector::SoftMax<TPrediction>);
			}
			//static Loss<TPrediction, TTarget> SoftStep() {
			//	return Loss<TPrediction, TTarget>(Function::SoftStep<TPrediction, TTarget>, Function::Derivative::SoftStep<TPrediction, TTarget>);
			//}
			//static Loss<TPrediction, TTarget> TanH() {
			//	return Loss<TPrediction, TTarget>(Function::TanH<TPrediction, TTarget>, Function::Derivative::TanH<TPrediction, TTarget>);
			//}
			//static Loss<TPrediction, TTarget> ReLU() {
			//	return Loss<TPrediction, TTarget>(Function::ReLU<TPrediction, TTarget>, Function::Derivative::ReLU<TPrediction, TTarget>);
			//}
			//static Loss<TPrediction, TTarget> LeakyReLU() {
			//	return Loss<TPrediction, TTarget>(Function::LeakyReLU<TPrediction, TTarget>, Function::Derivative::LeakyReLU<TPrediction, TTarget>);
			//}
			////static Loss<TPrediction, TTarget> PReLU() {
			////	return Loss<TPrediction, TTarget>(Function::PReLU<TPrediction, TTarget>, Function::Derivative::PReLU<TPrediction, TTarget>);
			////}
			//static Loss<TPrediction, TTarget> GELU() {
			//	return Loss<TPrediction, TTarget>(Function::GELU<TPrediction, TTarget>, Function::Derivative::GELU<TPrediction, TTarget>);
			//}
			//static Loss<TPrediction, TTarget> SELU() {
			//	return Loss<TPrediction, TTarget>(Function::SELU<TPrediction, TTarget>, Function::Derivative::SELU<TPrediction, TTarget>);
			//}
			//static Loss<TPrediction, TTarget> SoftPlus() {
			//	return Loss<TPrediction, TTarget>(Function::SoftPlus<TPrediction, TTarget>, Function::Derivative::SoftPlus<TPrediction, TTarget>);
			//}
			//static Loss<TPrediction, TTarget> Gausian() {
			//	return Loss<TPrediction, TTarget>(Function::Gaussian<TPrediction, TTarget>, Function::Derivative::Gaussian<TPrediction, TTarget>);
			//}
			//static Loss<TPrediction, TTarget> SiLU() {
			//	return Loss<TPrediction, TTarget>(Function::SiLU<TPrediction, TTarget>, Function::Derivative::SiLU<TPrediction, TTarget>);
			//}
			//static Loss<TPrediction, TTarget> SiL() {
			//	return Loss<TPrediction, TTarget>(Function::SiL<TPrediction, TTarget>, Function::Derivative::SiL<TPrediction, TTarget>);
			//}


		private:
			std::function<Eigen::MatrixX<TPrediction>(const Eigen::Ref<const Eigen::MatrixX<TPrediction>>&)> mProbabilityFunction;
			LossFunction<TPrediction, TTarget, Args...> mLossFunction;
			std::function<Eigen::MatrixX<TPrediction>(const Eigen::Ref<const Eigen::MatrixX<TPrediction>>&, const Eigen::Ref<const Eigen::MatrixX<TTarget>>&, Args...)> mLossPrimeFunction;
			std::tuple<Args...> mArgs;
		};

		template<typename TPrediction>
		static Loss<TPrediction, bool> BinaryCrossentropy{ Loss<TPrediction,bool>::BinaryCrossentropy() };
		template<typename TPrediction, typename TGamma , std::enable_if_t<std::is_integral<TGamma>::value || std::is_floating_point<TGamma>::value, int> = 0>
		static Loss<TPrediction, bool, TGamma> BinaryFocalCrossentropy{ Loss<TPrediction,bool, TGamma>::BinaryFocalCrossentropy() };
		template<typename TPrediction, typename TTarget = TPrediction, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
		static Loss<TPrediction, TTarget> CategoricalCrossentropy{ Loss<TPrediction,TTarget>::CategoricalCrossentropy() };
		template<typename TPrediction>
		static Loss<TPrediction, size_t> SparseCategoricalCrossentropy{ Loss<TPrediction,size_t>::SparseCategoricalCrossentropy() };
		//template<typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
		//static Loss<TPrediction, TTarget> SoftStep{ Function::SoftStep<TPrediction, TTarget>, Function::Derivative::SoftStep<TPrediction, TTarget> };
		//template<typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
		//static Loss<TPrediction, TTarget> TanH{ Function::TanH<TPrediction, TTarget>, Function::Derivative::TanH<TPrediction, TTarget> };
		//template<typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
		//static Loss<TPrediction, TTarget> ReLU{ Function::ReLU<TPrediction, TTarget>, Function::Derivative::ReLU<TPrediction, TTarget> };
		//template<typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
		//static Loss<TPrediction, TTarget> LeakyReLU{ Function::LeakyReLU<TPrediction, TTarget>, Function::Derivative::LeakyReLU<TPrediction, TTarget> };
		//template<typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
		//static Loss<TPrediction, TTarget> GELU{ Function::GELU<TPrediction, TTarget>, Function::Derivative::GELU<TPrediction, TTarget> };
		//template<typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
		//static Loss<TPrediction, TTarget> SELU{ Function::SELU<TPrediction, TTarget>, Function::Derivative::SELU<TPrediction, TTarget> };
		//template<typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
		//static Loss<TPrediction, TTarget> SoftPlus{ Function::SoftPlus<TPrediction, TTarget>, Function::Derivative::SoftPlus<TPrediction, TTarget> };
		//template<typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
		//static Loss<TPrediction, TTarget> Gausian{ Function::Gausian<TPrediction, TTarget>, Function::Derivative::Gausian<TPrediction, TTarget> };
		//template<typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
		//static Loss<TPrediction, TTarget> SiLU{ Function::SiLU<TPrediction, TTarget>, Function::Derivative::SiLU<TPrediction, TTarget> };
		//template<typename TPrediction, typename TTarget, std::enable_if_t<std::is_integral<TTarget>::value || std::is_floating_point<TTarget>::value, int> = 0>
		//static Loss<TPrediction, TTarget> SiL{ Function::SiL<TPrediction, TTarget>, Function::Derivative::SiL<TPrediction, TTarget> };

	} // namespace Loss
} // namespace Soren