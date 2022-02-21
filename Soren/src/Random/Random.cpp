#include "sorenpch.h"
#include "Random/Random.h"

namespace Soren {

	namespace Random {

		// Data

		std::random_device RandomDevice{};
		std::mt19937 RandomEngine{ RandomDevice() };

		// Chars

		//char Char()
		//{
		//    std::uniform_int_distribution<char> dis(CHAR_MIN, CHAR_MAX);
		//    return dis(RandomEngine);
		//}

		//char Char(char min, char max)
		//{
		//    std::uniform_int_distribution<char> dis(min, max);
		//    return dis(RandomEngine);
		//}

		//char Char(char max)
		//{
		//    std::uniform_int_distribution<char> dis(CHAR_MIN, max);
		//    return dis(RandomEngine);
		//}

		//wchar_t WChar()
		//{
		//    std::uniform_int_distribution<wchar_t> dis(WCHAR_MIN, WCHAR_MAX);
		//    return dis(RandomEngine);
		//}

		//wchar_t WChar(wchar_t min, wchar_t max)
		//{
		//    std::uniform_int_distribution<wchar_t> dis(min, max);
		//    return dis(RandomEngine);
		//}

		//wchar_t WChar(wchar_t max)
		//{
		//    std::uniform_int_distribution<wchar_t> dis(WCHAR_MIN, max);
		//    return dis(RandomEngine);
		//}

		//char16_t Char16()
		//{
		//    std::uniform_int_distribution<char16_t> dis(0, UINT_LEAST16_MAX);
		//    return dis(RandomEngine);
		//}

		//char16_t Char16(char16_t min, char16_t max)
		//{
		//    std::uniform_int_distribution<char16_t> dis(min, max);
		//    return dis(RandomEngine);
		//}

		//char16_t Char16(char16_t max)
		//{
		//    std::uniform_int_distribution<char16_t> dis(0, max);
		//    return dis(RandomEngine);
		//}

		//char32_t Char32()
		//{
		//    std::uniform_int_distribution<char32_t> dis(0, UINT_LEAST32_MAX);
		//    return dis(RandomEngine);
		//}

		//char32_t Char32(char32_t min, char32_t max)
		//{
		//    std::uniform_int_distribution<char32_t> dis(min, max);
		//    return dis(RandomEngine);
		//}

		//char32_t Char32(char32_t max)
		//{
		//    std::uniform_int_distribution<char32_t> dis(0, max);
		//    return dis(RandomEngine);
		//}

		// Shorts

		short Short()
		{
			std::uniform_int_distribution<short> dis(SHRT_MIN, SHRT_MAX);
			return dis(RandomEngine);
		}

		short Short(const short min, const short max)
		{
			std::uniform_int_distribution<short> dis(min, max);
			return dis(RandomEngine);
		}

		short Short(const short max)
		{
			std::uniform_int_distribution<short> dis(SHRT_MIN, max);
			return dis(RandomEngine);
		}

		unsigned short UShort()
		{
			std::uniform_int_distribution<unsigned short> dis(0, USHRT_MAX);
			return dis(RandomEngine);
		}

		unsigned short UShort(const unsigned short min, const unsigned short max)
		{
			std::uniform_int_distribution<unsigned short> dis(min, max);
			return dis(RandomEngine);
		}

		unsigned short UShort(const unsigned short max)
		{
			std::uniform_int_distribution<unsigned short> dis(0, max);
			return dis(RandomEngine);
		}

		// Unsigned Ints

		unsigned int UInt()
		{
			std::uniform_int_distribution<unsigned int> dis(0, UINT_MAX);
			return dis(RandomEngine);
		}

		unsigned int UInt(const unsigned int min, const unsigned int max)
		{
			std::uniform_int_distribution<unsigned int> dis(min, max);
			return dis(RandomEngine);
		}

		unsigned int UInt(const unsigned int max)
		{
			std::uniform_int_distribution<unsigned int> dis(0, max);
			return dis(RandomEngine);
		}

		// Ints

		int Int()
		{
			std::uniform_int_distribution<int> dis(INT_MIN, INT_MAX);
			return dis(RandomEngine);
		}

		int Int(const int min, const int max)
		{
			std::uniform_int_distribution<int> dis(min, max);
			return dis(RandomEngine);
		}

		int Int(const int max)
		{
			std::uniform_int_distribution<int> dis(INT_MIN, max);
			return dis(RandomEngine);
		}

		// Long

		long Long()
		{
			std::uniform_int_distribution<long> dis(LONG_MIN, LONG_MAX);
			return dis(RandomEngine);
		}

		long Long(const long max, const long min)
		{
			std::uniform_int_distribution<long> dis(max, min);
			return dis(RandomEngine);
		}

		long Long(const long max)
		{
			std::uniform_int_distribution<long> dis(LONG_MIN, max);
			return dis(RandomEngine);
		}

		unsigned long ULong()
		{
			std::uniform_int_distribution<unsigned long> dis(0, ULONG_MAX);
			return dis(RandomEngine);
		}

		unsigned long ULong(const unsigned long min, const unsigned long max)
		{
			std::uniform_int_distribution<unsigned long> dis(min, max);
			return dis(RandomEngine);
		}

		unsigned long ULong(const unsigned long max)
		{
			std::uniform_int_distribution<unsigned long> dis(0, max);
			return dis(RandomEngine);
		}

		long long LLong()
		{
			std::uniform_int_distribution<long long> dis(LLONG_MIN, LLONG_MAX);
			return dis(RandomEngine);
		}

		long long LLong(const long long max, const long long min)
		{
			std::uniform_int_distribution<long long> dis(max, min);
			return dis(RandomEngine);
		}

		long long LLong(const long long max)
		{
			std::uniform_int_distribution<long long> dis(LLONG_MIN, max);
			return dis(RandomEngine);
		}

		unsigned long long ULLong()
		{
			std::uniform_int_distribution<unsigned long long> dis(0, ULLONG_MAX);
			return dis(RandomEngine);
		}

		unsigned long long ULLong(const unsigned long long min, const unsigned long long max)
		{
			std::uniform_int_distribution<unsigned long long> dis(min, max);
			return dis(RandomEngine);
		}

		unsigned long long ULLong(const unsigned long long max)
		{
			std::uniform_int_distribution<unsigned long long> dis(0, max);
			return dis(RandomEngine);
		}

		// Floats

		float Float()
		{
			std::uniform_real_distribution<float> dis(FLT_MIN, FLT_MAX);
			return dis(RandomEngine);
		}

		float Float(const float min, const float max)
		{
			std::uniform_real_distribution<float> dis(min, max);
			return dis(RandomEngine);
		}

		float Float(const float max)
		{
			std::uniform_real_distribution<float> dis(FLT_MIN, max);
			return dis(RandomEngine);
		}

		// Doubles

		double Double()
		{
			std::uniform_real_distribution<double> dis(DBL_MIN, DBL_MAX);
			return dis(RandomEngine);
		}

		double Double(const double min, const double max)
		{
			std::uniform_real_distribution<double> dis(min, max);
			return dis(RandomEngine);
		}

		double Double(const double max)
		{
			std::uniform_real_distribution<double> dis(DBL_MIN, max);
			return dis(RandomEngine);
		}
	}

}
