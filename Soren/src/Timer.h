#pragma once

#include <chrono>

#include "Core.h"

namespace Soren {

	struct TimeSlice {

		std::chrono::duration<long long, std::nano> Time{};

		inline long long nanoseconds() const {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(Time).count();
		}
		inline long long microseconds() const {
			return std::chrono::duration_cast<std::chrono::microseconds>(Time).count();
		}
		inline long long milliseconds() const {
			return std::chrono::duration_cast<std::chrono::milliseconds>(Time).count();
		}
		inline long long seconds() const {
			return std::chrono::duration_cast<std::chrono::seconds>(Time).count();
		}
		inline int minutes() const {
			return std::chrono::duration_cast<std::chrono::minutes>(Time).count();
		}
		inline int hours() const {
			return std::chrono::duration_cast<std::chrono::hours>(Time).count();
		}

		friend std::ostream& operator<<(std::ostream& os, const TimeSlice& e)
		{
			// Default is milliseconds

			return os << e.milliseconds() << " ms";
		}

	};

	namespace Timer
	{

		// only works with void functions
		template <typename T, typename ...Args>
		TimeSlice Duration(T func, Args&&... args)
		{
			auto t1 = std::chrono::high_resolution_clock::now();

			func(std::forward<Args>(args)...);

			auto t2 = std::chrono::high_resolution_clock::now();
			auto t = t1 - t2;

			TimeSlice slice;
			slice.Time = t2 - t1;

			return slice;
		}

		template <typename T>
		TimeSlice Duration(T func)
		{
			auto t1 = std::chrono::high_resolution_clock::now();

			func();

			auto t2 = std::chrono::high_resolution_clock::now();

			TimeSlice slice;
			slice.Time = t2 - t1;

			return slice;
		}
	}

	class Stopwatch
	{
	public:
		Stopwatch() = default;

		TimeSlice Lap()
		{
			auto timePassed = std::chrono::high_resolution_clock::now() - m_StartTime;
			TimeSlice slice;
			slice.Time = timePassed;

			return slice;
		}


	private:
		std::chrono::steady_clock::time_point m_StartTime{ std::chrono::high_resolution_clock::now() };
	};
}
