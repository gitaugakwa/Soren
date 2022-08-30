#pragma once
#include "Log/Log.h"
#include "Random/Random.h"
#include <math.h>

// Identity
// Binary step
// Sigmoid
// TanH
// SQNL
// ArcTan
// ArSinH
// ElliotSig
// ISRU
// ISRLU
// PLU
// ReLU
// BReLU
// LeakyReLU
// PReLU
// RReLU -> PReLU with randomized gradients
// GELU
// ELU
// SELU
// SReLU
// APL
// SoftPlus
// BentIdentity
// SiLU
// SoftExponential
// SoftClipping
// Sinusoid
// Sinc
// Gaussian
// SQ-RBF


namespace Soren {

	namespace Activation {

		namespace Function {

			enum class ActivationFunction
			{
				Identity, BinaryStep, Sigmoid, Logistic, SoftSTep, TanH, SQNL,
				ArcTan, ArSinH, ElliotSig, ISRU, ISRLU,
				PLU, ReLU, BReLU, LeakyReLU, PReLU,
				RReLU, GELU, ELU, SELU, SReLU,
				APL, SoftPlus, BentIdentity, SoftExponential,
				SoftClipping, Sinusoid, Sinc, Gaussian, SQRBF, SiLU, SiL,
				SoftMax, MaxOut, DropOut
			};

			// not sure to take in by reference or value

			// Types

			// Identity
			template <typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput Identity(TInput data)
			{
				return data;
			}

			// Binary Step
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput BinaryStep(TInput data)
			{
				return std::clamp<TOutput>(data, 0, 1);
			}

			// Sigmoid
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput Sigmoid(TInput data)
			{
				// Always signed??
				return (1 / (1 + std::exp(-data)));
			}

			// Logistic
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput Logistic(TInput data)
			{
				return Sigmoid(data);
			}

			// SoftStep
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput SoftStep(TInput data)
			{
				return Sigmoid(data);
			}

			// TanH
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput TanH(TInput data)
			{
				return (std::tanh(data));
			}

			// SQNL
			// Fix
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput SQNL(TInput data)
			{
				if (std::is_integral<TInput>::value)
				{
					if (2 < data)
					{
						return (1);
					}
					else if (0 <= data && data <= 2)
					{
						return (data - ((std::pow(data, 2)) / 4));
					}
					else if (-2 <= data && data < 0)
					{
						return (data + ((std::pow(data, 2)) / 4));
					}
					else
					{
						return (-1);
					}
				}
				else if (std::is_floating_point<TInput>::value)
				{
					if (2 < data)
					{
						return (1);
					}
					else if (0 <= data && data <= 2)
					{
						return ((const double&)data - ((std::pow(data, 2)) / 4));
					}
					else if (-2 <= data && data < 0)
					{
						return ((const double&)data + ((std::pow(data, 2)) / 4));
					}
					else
					{
						return (-1);
					}
				}
			}

			// ArcTan
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput ArcTan(TInput data)
			{
				return (std::atan(data));
			}

			// ArSinH
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput ArSinH(TInput data)
			{
				return (std::asinh(data));
			}

			// ElliotSig
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput ElliotSig(TInput data)
			{
				if (std::is_signed<TInput>::value)
				{
					return ((const double&)data / (1 + std::abs((const double&)data)));
				}
				else
				{
					return ((const double&)data / (1 + data));
				}
			}

			// ISRU
			template <typename TInput, typename TGradient = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TInput ISRU(TInput data, const TGradient gradient = 1)
			{
				return (data / std::sqrt(1 + (gradient * std::pow(data, 2))));
			}

			// ISRLU
			template <typename TInput, typename TGradient, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TInput ISRLU(TInput data, const TGradient gradient = 1)
			{
				return (data < 0 ? (data / std::sqrt(1 + (gradient * std::pow(data, 2)))) : data);
			}

			// PLU
			template <typename TInput, typename TGradient = TInput, typename TVariance = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TInput PLU(TInput data, const TGradient gradient = 0.1, const TVariance variance = 1)
			{
				return (std::max(((gradient * ((const double&)data + variance)) - variance),
					(std::min((gradient * ((const double&)data - variance) + variance), (const double&)data))));
			}

			// ReLU
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput ReLU(TInput data)
			{
				return std::max<TInput>(0, data);
			}

			// BReLU
			//template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			//T BReLU(const T& data);


			// LeakyReLU
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput LeakyReLU(TInput data)
			{
				return (data < 0 ? (0.01 * data) : data);
			}

			// PReLU
			// Defaults to LeakyReLU gradient
			template <typename TInput, typename TGredient = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TInput PReLU(TInput data, const TInput gradient = 0.01)
			{
				return (data < 0 ? (gradient * data) : data);
			}

			// GELU
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput GELU(TInput data)
			{
				return (data * ((1 + std::erf(data / std::sqrt(2))) / 2));
			}

			// ELU
			template <typename TInput, typename TGradient = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TInput ELU(TInput data, const TGradient gradient = 1)
			{
				return (data <= 0 ? (gradient * (std::exp(data) - 1)) : data);
			}

			// SELU
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput SELU(TInput data)
			{
				return (1.0507 * (data < 0 ? (1.67326 * (std::exp(data) - 1)) : data));

			}

			// SoftPlus
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput SoftPlus(TInput data)
			{
				return (std::log(1 + std::exp(data)));
			}

			// BentIdentity
			template <typename TInput, typename TGradient = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TInput BentIdentity(TInput data, const TGradient gradient = 1)
			{
				return ((((gradient * std::sqrt(std::pow(data, 2) + 1)) - gradient) / 2) + data);
			}

			// SoftClipping
			// Fix
			template <typename TInput, typename TGradient = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TInput SoftClipping(TInput data, const TGradient gradient = 1)
			{
				return ((1 / gradient) * (std::log((1 + std::exp(gradient * data)) /
					(1 + std::exp(gradient * (data - 1))))));
			}

			// Sinusoid
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput Sinusoid(TInput data)
			{
				return (std::sin(data));
			}

			// Sinc
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput Sinc(TInput data)
			{
				return (data == 0 ? 1 : ((std::sin(data)) / data));
			}

			// Gaussian
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput Gaussian(TInput data)
			{
				return (std::exp(-(std::pow(data, 2))));
			}

			// SiLU
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput SiLU(TInput data)
			{
				return (data / (1 + std::exp(-data)));
			}

			// SiL
			template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static constexpr TOutput SiL(TInput data)
			{
				return SiLU(data);
			}

			namespace Vector {

				// Identity
				template <typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::MatrixX<TOutput> Identity(const Eigen::Ref<const Eigen::MatrixX<TInput>>& data)
				{
					return data.cast<TOutput>();
				}

				// Binary Step
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> BinaryStep(const Eigen::Ref<const Eigen::VectorX<TInput>>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::BinaryStep<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// Sigmoid
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::MatrixX<TOutput> Sigmoid(const Eigen::MatrixX<TInput>& data)
				{
					return data.cast<TOutput>().unaryExpr(Function::Sigmoid<TOutput, TOutput>);
					std::vector result;
					result.reserve(data.rows());

					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::Sigmoid<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// Logistic
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> Logistic(const Eigen::VectorX<TInput>& data)
				{
					return Sigmoid(data);
				}

				// SoftStep
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> SoftStep(const Eigen::VectorX<TInput>& data)
				{
					return Sigmoid(data);
				}

				// TanH
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> TanH(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::TanH<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// SQNL
				// Fix
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> SQNL(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::SQNL<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// ArcTan
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> ArcTan(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::ArcTan<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// ArSinH
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> ArSinH(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::ArSinH<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// ElliotSig
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> ElliotSig(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::ElliotSig<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// ISRU
				template <typename TInput, typename TOutput = double, typename TGradient = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> ISRU(const Eigen::VectorX<TInput>& data, const TGradient gradient = 1)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::ISRU<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// ISRLU
				template <typename TInput, typename TOutput = double, typename TGradient, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> ISRLU(const Eigen::VectorX<TInput>& data, const TGradient gradient = 1)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::ISRLU<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// PLU
				template <typename TInput, typename TOutput = double, typename TGradient = TInput, typename TVariance = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> PLU(const Eigen::VectorX<TInput>& data, const TGradient gradient = 0.1, const TVariance variance = 1)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::PLU<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// ReLU
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> ReLU(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::ReLU<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// BReLU
				//template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				//T BReLU(const T& data);


				// LeakyReLU
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> LeakyReLU(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::LeakyReLU<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// PReLU
				// Defaults to LeakyReLU gradient
				template <typename TInput, typename TOutput = double, typename TGredient = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> PReLU(const Eigen::VectorX<TInput>& data, const TInput gradient = 0.01)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::PReLU<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// GELU
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> GELU(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::GELU<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// ELU
				template <typename TInput, typename TOutput = double, typename TGradient = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> ELU(const Eigen::VectorX<TInput>& data, const TGradient gradient = 1)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::ELU<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// SELU
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> SELU(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::SELU<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());

				}

				// SoftPlus
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> SoftPlus(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::SoftPlus<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// BentIdentity
				template <typename TInput, typename TOutput = double, typename TGradient = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> BentIdentity(const Eigen::VectorX<TInput>& data, const TGradient gradient = 1)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::BentIdentity<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// SoftClipping
				// Fix
				template <typename TInput, typename TOutput = double, typename TGradient = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> SoftClipping(const Eigen::VectorX<TInput>& data, const TGradient gradient = 1)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::SoftClipping<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// Sinusoid
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> Sinusoid(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::Sinusoid<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// Sinc
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> Sinc(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::Sinc<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());

				}

				// Gaussian
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> Gaussian(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::Gaussian<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// SiLU
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> SiLU(const Eigen::VectorX<TInput>& data)
				{
					std::vector<TOutput> result;
					result.reserve(data.rows());
					std::transform(data.begin(), data.end(), std::back_inserter(result), Function::SiLU<TInput, TOutput>);
					return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(result.data(), result.size());
				}

				// SiL
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> SiL(const Eigen::VectorX<TInput>& data)
				{
					return SiLU(data);
				}


				// SoftMax
				//template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				//static constexpr std::vector<TOutput> SoftMax(const Eigen::VectorX<TInput>& data)
				//{
				//	//oneapi::tbb::simple_partitioner partitioner;
				//	std::vector<TOutput> valueVec(data.rows());
				//	std::vector<double> expVec(data.rows());
				//	double totalExp{ 0 };

				//	//double totalExp = oneapi::tbb::parallel_reduce(oneapi::tbb::blocked_range<size_t>(0, data.rows(), 50),
				//	//	0.0,
				//	//	[&](const oneapi::tbb::blocked_range<size_t>& r, double total) {
				//	//		auto begin = r.begin();
				//	//		auto end = r.end();
				//	//		for (size_t iValue = begin; iValue < end; iValue++) {
				//	//			total += expVec[iValue];
				//	//		}
				//	//		return total;
				//	//	}, std::plus<double>(),  partitioner);

				//	for (size_t iValue = 0; iValue < data.rows(); iValue++) {
				//		auto exp = std::exp(data[iValue]);

				//		expVec[iValue] = exp;
				//		totalExp += exp;
				//	}

				//	for (size_t iValue = 0; iValue < data.rows(); iValue++) {
				//		valueVec[iValue] = (expVec[iValue] / totalExp);
				//	}

				//	return valueVec;
				//}
				template <typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::MatrixX<TOutput> SoftMax(const Eigen::Ref<const Eigen::MatrixX<TInput>>& data)
				{
					Eigen::MatrixX<TOutput> exp{ data.cast<TOutput>().array().exp() };
					return exp.array().rowwise() / exp.colwise().sum().array();
				}
				//template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				//static constexpr Eigen::VectorX<TOutput> SoftMaxMatrix(const Eigen::VectorX<TInput>& data)
				//{
				//	return Eigen::Map<Eigen::VectorX<TOutput>, Eigen::Unaligned>(SoftMax(data).data(), data.rows());
				//}

				// MaxOut
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput MaxOut(const Eigen::VectorX<TInput>& data)
				{
					return *std::max_element(data.begin(), data.end());
				}

				// DropOut
				template <typename TInput, typename TOutput = double, typename TRate = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr Eigen::VectorX<TOutput> DropOut(const Eigen::VectorX<TInput>& data, TRate rate)
				{
					Eigen::VectorX<TOutput> valueVec(data.rows());

					for (size_t iValue = 0; iValue < data.rows(); iValue++) {
						if (Random::Double(0, 1) < rate) {
							valueVec(iValue) = 0;
						}
						else {
							valueVec(iValue) = data[iValue] / rate;
						}
					}

					return valueVec;
				}

			}

			namespace Derivative {

				enum class ActivationFunctionDerivative
				{
					Identity, BinaryStep, Sigmoid, Logistic, SoftSTep, TanH,
					ReLU, LeakyReLU, PReLU,
					GELU, ELU, SELU, SReLU,
					SoftPlus,
					Gaussian, SiLU, SiL,
					SoftMax
				};

				// Identity
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput Identity(TInput data)
				{
					data;
					return (1);
				}

				// Binary Step
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput BinaryStep(TInput data)
				{
					if (data != 0) {
						return 0;
					}
					throw std::invalid_argument("BinaryStep Derivative is undefined for data = 0");
				}

				// Sigmoid
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput Sigmoid(TInput data)
				{
					TOutput value = Function::Sigmoid<TInput, TOutput>(data);
					return (value * (1 - value));
				}

				// Logistic
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput Logistic(TInput data)
				{
					return Sigmoid(data);
				}

				// SoftStep
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput SoftStep(TInput data)
				{
					return Sigmoid(data);
				}

				// TanH
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput TanH(TInput data)
				{
					return(1 - std::pow(std::tanh(data), 2));
				}

				// ReLU
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput ReLU(TInput data)
				{
					if (data < 0) {
						return 0;
					}
					// We can use >= even though it is technically inaccurate
					if (data >= 0) {
						return 1;
					}
					throw std::invalid_argument("ReLU Derivative is undefined for data = 0");
				}

				// LeakyReLU
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput LeakyReLU(TInput data)
				{
					if (data < 0) {
						return 0.01;
					}
					return 1;
				}

				// PReLU
				// Defaults to LeakyReLU gradient
				template <typename TInput, typename TGredient = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TInput PReLU(TInput data, const TInput gradient = 0.01)
				{
					if (data < 0) {
						return gradient;
					}
					return 1;
				}

				// GELU
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput GELU(TInput data)
				{
					return ((std::erf(data / std::sqrt(2)) + 1) / (2)) + ((data * std::exp(-(std::pow(data, 2) / 2))) / (std::sqrt(2) * std::sqrt(std::acos(-1)))); // std::acos(-1) = PI
				}

				// ELU
				template <typename TInput, typename TGradient = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TInput ELU(TInput data, const TGradient gradient = 1)
				{
					if (data < 0) {
						return gradient * std::exp(data);
					}
					if (data > 0) {
						return 1;
					}
					if (data == 0 && gradient = 1) {
						return 1;
					}
					throw std::invalid_argument("ELU Derivative is undefined");
				}

				// SELU
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput SELU(TInput data)
				{
					if (data < 0) {
						return 1.67326 * std::exp(data);
					}
					return 1;

				}

				// SoftPlus
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput SoftPlus(TInput data)
				{
					return (1 / (1 + std::exp(-data)));
				}

				// Gaussian
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput Gaussian(TInput data)
				{
					return ((-2) * data * std::exp(-(std::pow(data, 2))));
				}

				// SiLU
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput SiLU(TInput data)
				{
					auto expValue = std::exp(-data);
					return ((1 + expValue + (data * expValue)) / std::pow(1 + expValue, 2));
				}

				// SiL
				template <typename TInput, typename TOutput = double, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
				static constexpr TOutput SiL(TInput data)
				{
					return SiLU(data);
				}

				namespace Vector {

					// Identity
					template <typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
					static constexpr std::vector<std::shared_ptr<Eigen::MatrixX<TOutput>>> Identity(const Eigen::Ref<const Eigen::MatrixX<TInput>>& data)
					{
						size_t width = (size_t)data.cols();
						size_t height = (size_t)data.rows();
						std::shared_ptr<Eigen::MatrixX<TOutput>> identity = std::make_shared<Eigen::MatrixX<TOutput>>(Eigen::MatrixX<TOutput>::Identity(height, height));
						std::vector<std::shared_ptr<Eigen::MatrixX<TOutput>>> result(width, identity);
						return result;
					}

					// SoftMax
					template <typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
					static constexpr std::vector<std::shared_ptr<Eigen::MatrixX<TOutput>>> SoftMax(const Eigen::Ref<const Eigen::MatrixX<TInput>>& data)
					{
						size_t width = (size_t)data.cols();
						//size_t height = (size_t)data.rows();
						Eigen::MatrixX<TOutput> softMax = Function::Vector::SoftMax<TInput, TOutput>(data);
						Eigen::MatrixX<TOutput> DSoftMax;
						//Eigen::MatrixX<TOutput> softMax{ Function::Vector::SoftMax<TInput,TOutput>(data) };
						//Eigen::MatrixX<TOutput> DSoftMax{ (softMax * softMax.transpose()) * -1 };

						//DSoftMax.diagonal().noalias() = softMax.unaryExpr([](auto val) { return (val * (1 - val)); });

						std::vector<std::shared_ptr<Eigen::MatrixX<TOutput>>> result(width);
						std::transform(std::execution::par_unseq, softMax.colwise().begin(), softMax.colwise().end(), result.begin(), [&](const auto& softMax) {
							//Eigen::MatrixX<TOutput> softMax{ Function::Vector::SoftMax<TInput,TOutput>(column) };
							DSoftMax.noalias() = (softMax * softMax.transpose()) * -1;

							//DSoftMax.diagonal().noalias() = softMax.unaryExpr([](auto val) { return (val * (1 - val)); });
							DSoftMax.diagonal() = softMax.array() * (1 - softMax.array());
							//return DSoftMax;
							return std::make_shared<Eigen::MatrixX<TOutput>>(DSoftMax);
							});
						//SOREN_CORE_INFO((result.front()));
						//SOREN_CORE_INFO((result.back()));

						return result;
						//Eigen::MatrixX<TOutput> DSoftMax{ 0, 0};
						//return DSoftMax;
					}
				}
			} // Derivative
		} // Function

		template<typename TInput, typename TOutput = TInput, typename TPrimeInput=TInput,typename TPrimeOutput = TOutput >
		class ActivatorBase {
		public:
			virtual ~ActivatorBase() {}
			virtual TOutput operator ()(TInput input) const {
				SOREN_CORE_WARN("ActivationBase {0}", __FUNCTION__);
				return input;
			};
			virtual TPrimeOutput prime(TPrimeInput input) const {
				input;
				SOREN_CORE_WARN("ActivationBase {0}", __FUNCTION__);
				return TPrimeOutput();
			};
		};

		template<typename TInput, typename TOutput = TInput, typename ...Args>
		class Activator: public ActivatorBase<TInput, TOutput> {
		public:

			Activator(std::function<TOutput(TInput, Args...)> function, std::function<TOutput(TInput, Args...)> derivative, Args... args)
				: mActivationFunction(function), mActivationPrimeFunction(derivative), mArgs(std::forward<Args>(args)...)
			{}

			TOutput operator ()(TInput input) const override {
				return std::apply(mActivationFunction, std::make_tuple(input, std::get<Args>(std::forward<std::tuple<Args...>>(mArgs))...));
				//return mActivationFunction(input, std::forward<Args>(mArgs)...);
			}

			TOutput prime(TInput input) const override {
				return std::apply(mActivationPrimeFunction, std::make_tuple(input, std::get<Args>(std::forward<std::tuple<Args...>>(mArgs))...));
				//return mActivationPrimeFunction(input,std::forward<Args>(mArgs)...);
			}

			static Activator<TInput, TOutput, Args...> Identity() {
				return Activator<TInput, TOutput, Args...>{Function::Identity<TInput>, Function::Derivative::Identity<TInput>};
			}
			static Activator<TInput, TOutput, Args...> BinaryStep() {
				return Activator<TInput, TOutput, Args...>(Function::BinaryStep<TInput>, Function::Derivative::BinaryStep<TInput>);
			}
			static Activator<TInput, TOutput, Args...> Sigmoid() {
				return Activator<TInput, TOutput, Args...>(Function::Sigmoid<TInput>, Function::Derivative::Sigmoid<TInput>);
			}
			static Activator<TInput, TOutput, Args...> Logistic() {
				return Activator<TInput, TOutput, Args...>(Function::Logistic<TInput>, Function::Derivative::Logistic<TInput>);
			}
			static Activator<TInput, TOutput, Args...> SoftStep() {
				return Activator<TInput, TOutput, Args...>(Function::SoftStep<TInput>, Function::Derivative::SoftStep<TInput>);
			}
			static Activator<TInput, TOutput, Args...> TanH() {
				return Activator<TInput, TOutput, Args...>(Function::TanH<TInput>, Function::Derivative::TanH<TInput>);
			}
			static Activator<TInput, TOutput, Args...> ReLU() {
				return Activator<TInput, TOutput, Args...>(Function::ReLU<TInput>, Function::Derivative::ReLU<TInput>);
			}
			static Activator<TInput, TOutput, Args...> LeakyReLU() {
				return Activator<TInput, TOutput, Args...>(Function::LeakyReLU<TInput>, Function::Derivative::LeakyReLU<TInput>);
			}
			static Activator<TInput, TOutput, Args...> PReLU() {
				return Activator<TInput, TOutput, Args...>(Function::PReLU<TInput>, Function::Derivative::PReLU<TInput>);
			}
			static Activator<TInput, TOutput, Args...> GELU() {
				return Activator<TInput, TOutput, Args...>(Function::GELU<TInput>, Function::Derivative::GELU<TInput>);
			}
			static Activator<TInput, TOutput, Args...> SELU() {
				return Activator<TInput, TOutput, Args...>(Function::SELU<TInput>, Function::Derivative::SELU<TInput>);
			}
			static Activator<TInput, TOutput, Args...> SoftPlus() {
				return Activator<TInput, TOutput, Args...>(Function::SoftPlus<TInput>, Function::Derivative::SoftPlus<TInput>);
			}
			static Activator<TInput, TOutput, Args...> Gausian() {
				return Activator<TInput, TOutput, Args...>(Function::Gaussian<TInput>, Function::Derivative::Gaussian<TInput>);
			}
			static Activator<TInput, TOutput, Args...> SiLU() {
				return Activator<TInput, TOutput, Args...>(Function::SiLU<TInput>, Function::Derivative::SiLU<TInput>);
			}
			static Activator<TInput, TOutput, Args...> SiL() {
				return Activator<TInput, TOutput, Args...>(Function::SiL<TInput>, Function::Derivative::SiL<TInput>);
			}


		private:
			std::function<TOutput(TInput, Args...)> mActivationFunction;
			std::function<TOutput(TInput, Args...)> mActivationPrimeFunction;
			std::tuple<Args...> mArgs;
		};

		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput, TOutput> Identity{ Activator<TInput,TOutput>::Identity()};
		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput,TOutput> BinaryStep{ Activator<TInput,TOutput>::BinaryStep()};
		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput,TOutput> Sigmoid {Activator<TInput,TOutput>::Sigmoid()};
		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput,TOutput> Logistic {Activator<TInput,TOutput>::Logistic()};
		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput,TOutput> SoftStep {Activator<TInput,TOutput>::SoftStep()};
		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput,TOutput> TanH {Activator<TInput,TOutput>::TanH()};
		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput,TOutput> ReLU {Activator<TInput,TOutput>::ReLU()};
		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput,TOutput> LeakyReLU {Activator<TInput,TOutput>::LeakyReLU()};
		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput,TOutput> GELU {Activator<TInput,TOutput>::GELU()};
		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput,TOutput> SELU {Activator<TInput,TOutput>::SELU()};
		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput,TOutput> SoftPlus {Activator<TInput,TOutput>::SoftPlus()};
		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput,TOutput> Gaussian {Activator<TInput,TOutput>::Gaussian()};
		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput,TOutput> SiLU {Activator<TInput,TOutput>::SiLU()};
		template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
		static Activator<TInput, TOutput> SiL{ Activator<TInput,TOutput>::SiL()};

		namespace Vector {

			template<typename TInput, typename TOutput = TInput, typename TPrimeInput = TInput, typename TPrimeOutput = TOutput >
			using ActivatorBase = Activation::ActivatorBase< const Eigen::Ref<const Eigen::MatrixX<TInput>>&, Eigen::MatrixX<TOutput>, const Eigen::Ref<const Eigen::MatrixX<TInput>>&, std::vector<std::shared_ptr<Eigen::MatrixX<TOutput>>> >;

			template<typename TInput, typename TOutput = TInput, typename ...Args>
			class Activator: public ActivatorBase<TInput, TOutput> {
			public:

				Activator(std::function<Eigen::MatrixX<TOutput>(const Eigen::MatrixX<TInput>&, Args...)> function, std::function<std::vector<std::shared_ptr<Eigen::MatrixX<TOutput>>>(const Eigen::MatrixX<TInput>&, Args...)> derivative, Args... args)
					: mActivationFunction(function), mActivationPrimeFunction(derivative), mArgs(std::forward<Args>(args)...)
				{}

				Eigen::MatrixX<TOutput> operator ()(const Eigen::Ref<const Eigen::MatrixX<TInput>>& input) const override {
					return mActivationFunction(input, std::forward<Args>(mArgs)...);
				}
				
				std::vector<std::shared_ptr<Eigen::MatrixX<TOutput>>> prime(const Eigen::Ref<const Eigen::MatrixX<TInput>>& input) const override {
					return mActivationPrimeFunction(input, std::forward<Args>(mArgs)...);
				}

				static Activator<TInput, TOutput> Identity() {
					return Activator<TInput, TOutput>(Function::Vector::Identity<TInput>, Function::Derivative::Vector::Identity<TInput>);
				}
				static Activator<TInput, TOutput> SoftMax() {
					return Activator<TInput, TOutput>(Function::Vector::SoftMax<TInput>, Function::Derivative::Vector::SoftMax<TInput>);
				}
			


			private:
				std::function<Eigen::MatrixX<TOutput>(const Eigen::MatrixX<TInput>&, Args...)> mActivationFunction;
				std::function<std::vector<std::shared_ptr<Eigen::MatrixX<TOutput>>>(const Eigen::MatrixX<TInput>&, Args...)> mActivationPrimeFunction;
				std::tuple<Args...> mArgs;
			};

			template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static Activator<TInput,TOutput> Identity{ Activator<TInput,TOutput>::Identity() };
			template<typename TInput, typename TOutput = TInput, std::enable_if_t<std::is_integral<TInput>::value || std::is_floating_point<TInput>::value, int> = 0>
			static Activator<TInput, TOutput> SoftMax{ Activator<TInput,TOutput>::SoftMax() };
			
		} // namespace Vector

	} // namespace Activation
} // namespace Soren
