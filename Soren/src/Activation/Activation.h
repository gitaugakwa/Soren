#pragma once
#include "Core.h"
#include <cmath>

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

		enum class ActivationFunc
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
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T Identity(const T data)
		{
			return (T)(data);
		}

		// Binary Step
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T BinaryStep(const T data)
		{
			return std::clamp<T>(data, 0, 1);
		}

		// Sigmoid
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T Sigmoid(const T data)
		{
			// Always signed??
			return (T)(1 / (1 + std::exp(-data)));
		}

		// Logistic
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T Logistic(const T data)
		{
			return Sigmoid(data);
		}

		// SoftStep
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T SoftStep(const T data)
		{
			return Sigmoid(data);
		}

		// TanH
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T TanH(const T data)
		{
			return(T)(std::tanh(data));
		}

		// SQNL
		// Fix
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T SQNL(const T data)
		{
			if (std::is_integral<T>::value)
			{
				if (2 < data)
				{
					return (T)(1);
				}
				else if (0 <= data && data <= 2)
				{
					return (T)(data - ((std::pow(data, 2)) / 4));
				}
				else if (-2 <= data && data < 0)
				{
					return (T)(data + ((std::pow(data, 2)) / 4));
				}
				else
				{
					return (T)(-1);
				}
			}
			else if (std::is_floating_point<T>::value)
			{
				if (2 < data)
				{
					return (T)(1);
				}
				else if (0 <= data && data <= 2)
				{
					return (T)((const double&)data - ((std::pow(data, 2)) / 4));
				}
				else if (-2 <= data && data < 0)
				{
					return (T)((const double&)data + ((std::pow(data, 2)) / 4));
				}
				else
				{
					return (T)(-1);
				}
			}
		}

		// ArcTan
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T ArcTan(const T data)
		{
			return (T)(std::atan(data));
		}

		// ArSinH
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T ArSinH(const T data)
		{
			return (T)(std::asinh(data));
		}

		// ElliotSig
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T ElliotSig(const T data)
		{
			if (std::is_signed<T>::value)
			{
				return (T)((const double&)data / (1 + std::abs((const double&)data)));
			}
			else
			{
				return (T)((const double&)data / (1 + data));
			}
		}

		// ISRU
		template <typename TData,typename TGradient = TData, std::enable_if_t<std::is_integral<TData>::value || std::is_floating_point<TData>::value, int> = 0>
		static constexpr TData ISRU(const TData data, const TGradient gradient = 1)
		{
			return (T)(data / std::sqrt(1 + (gradient * std::pow(data, 2))));
		}

		// ISRLU
		template <typename TData, typename TGradient, std::enable_if_t<std::is_integral<TData>::value || std::is_floating_point<TData>::value, int> = 0>
		static constexpr TData ISRLU(const TData data, const TGradient gradient = 1)
		{
			return (T)(data < 0 ? (data / std::sqrt(1 + (gradient * std::pow(data, 2)))) : data);
		}

		// PLU
		template <typename TData, typename TGradient = TData, typename TVariance = TData, std::enable_if_t<std::is_integral<TData>::value || std::is_floating_point<TData>::value, int> = 0>
		static constexpr TData PLU(const TData data, const TGradient gradient = 0.1, const TVariance variance = 1)
		{
			return (T)(std::max(((gradient * ((const double&)data + variance)) - variance),
				(std::min((gradient * ((const double&)data - variance) + variance), (const double&)data))));
		}

		// ReLU
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T ReLU(const T data)
		{
			return std::max<T>(0, data);
		}

		// BReLU
		//template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		//T BReLU(const T& data);


		// LeakyReLU
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T LeakyReLU(const T data)
		{
			return (T)(data < 0 ? (0.01 * data) : data);
		}

		// PReLU
		// Defaults to LeakyReLU gradient
		template <typename TData,typename TGredient = TData, std::enable_if_t<std::is_integral<TData>::value || std::is_floating_point<TData>::value, int> = 0>
		static constexpr TData PReLU(const TData data, const TData gradient = 0.01)
		{
			return (T)(data < 0 ? (gradient * data) : data);
		}

		// GELU
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T GELU(const T data)
		{
			return (T)(data * ((1 + std::erf(data / std::sqrt(2))) / 2));
		}

		// ELU
		template <typename TData,typename TGradient = TData, std::enable_if_t<std::is_integral<TData>::value || std::is_floating_point<TData>::value, int> = 0>
		static constexpr TData ELU(const TData data, const TGradient gradient = 1)
		{
			return (T)(data <= 0 ? (gradient * (std::exp(data) - 1)) : data);
		}

		// SELU
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T SELU(T data)
		{
			return (T)(1.0507 * (data < 0 ? (1.67326 * (std::exp(data) - 1)) : data));

		}

		// SoftPlus
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T SoftPlus(const T data)
		{
			return (T)(std::log(1 + std::exp(data)));
		}

		// BentIdentity
		template <typename TData, typename TGradient = TData, std::enable_if_t<std::is_integral<TData>::value || std::is_floating_point<TData>::value, int> = 0>
		static constexpr TData BentIdentity(const TData data, const TGradient gradient = 1)
		{
			return (T)((((gradient * std::sqrt(std::pow(data, 2) + 1)) - gradient) / 2) + data);
		}

		// SoftClipping
		// Fix
		template <typename TData, typename TGradient = TData, std::enable_if_t<std::is_integral<TData>::value || std::is_floating_point<TData>::value, int> = 0>
		static constexpr TData SoftClipping(const TData data, const TGradient gradient = 1)
		{
			return (T)((1 / gradient) * (std::log((1 + std::exp(gradient * data)) / 
				(1 + std::exp(gradient * (data - 1))))));
		}

		// Sinusoid
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T Sinusoid(const T data)
		{
			return (T)(std::sin(data));
		}

		// Sinc
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T Sinc(const T data)
		{
			return (T)(data == 0 ? 1 : ((std::sin(data)) / data));
		}

		// Gaussian
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T Gaussian(const T data)
		{
			return (T)(std::exp(-(std::pow(data, 2))));
		}
		
		// SiLU
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T SiLU(const T data)
		{
			return (T)(data / (1 + std::exp(-data)));
		}
		
		// SiL
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T SiL(const T data)
		{
			return SiLU(data);
		}
		
		// SoftMax
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr std::vector<T> SoftMax(const std::vector<T> data)
		{
			std::vector<T> expVec(data.size());
			std::vector<T> valueVec(data.size());
			T totalExp = 0;

			for (size_t iValue = 0; iValue < data.size(); iValue++) {
				auto exp = std::exp(data[iValue]);

				expVec[iValue] = exp;
				totalExp += exp;
			}

			for (size_t iValue = 0; iValue < data.size(); iValue++) {
				valueVec[iValue] = (expVec[iValue] / totalExp);
			}

			return valueVec ;
		}

		// MaxOut
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T MaxOut(const std::vector<T> data)
		{
			return *std::max_element(data.begin(), data.end());
		}

		// DropOut
		template <typename TData, typename TRate = TData, std::enable_if_t<std::is_integral<TData>::value || std::is_floating_point<TData>::value, int> = 0>
		static constexpr std::vector<TData> DropOut(const std::vector<TData> data, TRate rate)
		{
			std::vector<TData> valueVec(data.size());

			for (size_t iValue = 0; iValue < data.size(); iValue++) {
				if (Random::Double(0, 1) < rate) {
					valueVec[iValue] = 0;
				}
				else {
					valueVec[iValue] = data[iValue] / rate;
				}
			}

			return valueVec;
		}

	};
}
