#pragma once
#include "Core.h"
#include <cmath>

#include "Type.h"

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
			Identity, BinaryStep, Sigmoid, TanH, SQNL,
			ArcTan, ArSinH, ElliotSig, ISRU, ISRLU,
			PLU, ReLU, BReLU, LeakyReLU, PReLU,
			RReLU, GELU, ELU, SELU, SReLU,
			APL, SoftPlus, BentIdentity, SiLU, SoftExponential,
			SoftClipping, Sinusoid, Sinc, Gaussian, SQRBF
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
			return (T)(data >= 0 ? 1 : 0);
		}

		// Sigmoid
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T Sigmoid(const T data)
		{
			// Always signed??
			return (T)(1 / (1 + std::exp(-data)));
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
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T ISRU(const T data, const Gradient_t gradient = 1)
		{
			return (T)(data / std::sqrt(1 + (gradient * std::pow(data, 2))));
		}

		// ISRLU
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T ISRLU(const T data, const Gradient_t gradient = 1)
		{
			return (T)(data < 0 ? (data / std::sqrt(1 + (gradient * std::pow(data, 2)))) : data);
		}

		// PLU
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T PLU(const T data, const Gradient_t gradient = 0.1, const Variance_t variance = 1)
		{
			return (T)(std::max(((gradient * ((const double&)data + variance)) - variance),
				(std::min((gradient * ((const double&)data - variance) + variance), (const double&)data))));
		}

		// ReLU
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T ReLU(const T data)
		{
			return (T)(data <= 0 ? 0 : data);
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
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T PReLU(const T data, const Gradient_t gradient = 0.01)
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
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T ELU(const T data, const Gradient_t gradient = 1)
		{
			return (T)(data <= 0 ? (gradient * (std::exp(data) - 1)) : data);
		}

		// SELU
		//template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		//T SELU(T data);

		// SoftPlus
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T SoftPlus(const T data)
		{
			return (T)(std::log(1 + std::exp(data)));
		}

		// BentIdentity
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T BentIdentity(const T data, const Gradient_t gradient = 1)
		{
			return (T)((((gradient * std::sqrt(std::pow(data, 2) + 1)) - gradient) / 2) + data);
		}

		// SoftClipping
		// Fix
		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		static constexpr T SoftClipping(const T data, const Gradient_t gradient = 1)
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

	};
}
