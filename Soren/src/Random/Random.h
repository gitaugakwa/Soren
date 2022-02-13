#pragma once

// caused warning C4003: not enough arguments for function-like macro invocation 'min'
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include "Core.h"

#include <random>
#include <algorithm>
#include <functional>
#include <limits>


namespace Soren {

	namespace Random {

		// Char
		//char Char();
		//char Char(char min, char max);
		//char Char(char max);

		////char8_t Char8();
		////char8_t Char8(char8_t min, char8_t max);
		////char8_t Char8(char8_t max);

		//wchar_t WChar();
		//wchar_t WChar(wchar_t min, wchar_t max);
		//wchar_t WChar(wchar_t max);

		//char16_t Char16();
		//char16_t Char16(char16_t min, char16_t max);
		//char16_t Char16(char16_t max);

		//char32_t Char32();
		//char32_t Char32(char32_t min, char32_t max);
		//char32_t Char32(char32_t max);

		// Shorts
		short Short();
		short Short(const short min, const short max);
		short Short(const short max);

		unsigned short UShort();
		unsigned short UShort(const unsigned short min, const unsigned short max);
		unsigned short UShort(const unsigned short max);

		// Ints
		int Int();
		int Int(const int min, const int max);
		int Int(const int max);

		unsigned int UInt();
		unsigned int UInt(const unsigned int min, const unsigned int max);
		unsigned int UInt(const unsigned int max);

		// Longs
		long Long();
		long Long(const long min, const long max);
		long Long(const long max);

		unsigned long ULong();
		unsigned long ULong(const unsigned long min, const unsigned long max);
		unsigned long ULong(const unsigned long max);

		long long LLong();
		long long LLong(const long long min, const long long max);
		long long LLong(const long long max);

		unsigned long long ULLong();
		unsigned long long ULLong(const unsigned long long min, const unsigned long long max);
		unsigned long long ULLong(const unsigned long long max);

		// Floats
		float Float();
		float Float(const float min, const float max);
		float Float(const float max);

		// Doubles
		double Double();
		double Double(const double min, const double max);
		double Double(const double max);

		extern std::random_device RandomDevice;
		extern std::mt19937 RandomEngine;

		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		T Number()
		{
			if constexpr (std::is_integral<T>::value)
			{
				std::uniform_int_distribution<T> dis(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
				return dis(RandomEngine);
			}
			else if (std::is_floating_point<T>::value)
			{
				std::uniform_real_distribution<T> dis(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
				return dis(RandomEngine);
			}
		}

		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		T Number(const T max)
		{
			if constexpr (std::is_integral<T>::value)
			{
				std::uniform_int_distribution<T> dis(std::numeric_limits<T>::min(), max);
				return dis(RandomEngine);
			}
			else if constexpr (std::is_floating_point<T>::value)
			{
				std::uniform_real_distribution<T> dis(std::numeric_limits<T>::min(), max);
				return dis(RandomEngine);
			}
		}

		template <typename T, std::enable_if_t<std::is_integral<T>::value || std::is_floating_point<T>::value, int> = 0>
		T Number(const T min, const T max)
		{
			if constexpr (std::is_integral<T>::value)
			{
				std::uniform_int_distribution<T> dis(min, max);
				return dis(RandomEngine);
			}
			else if constexpr (std::is_floating_point<T>::value)
			{
				std::uniform_real_distribution<T> dis(min, max);
				return dis(RandomEngine);
			}
		}

	};

}
