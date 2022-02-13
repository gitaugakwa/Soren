#pragma once

#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Soren {
	class SOREN_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {return s_ClientLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetTimeLogger() {return s_TimeLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::shared_ptr<spdlog::logger> s_TimeLogger;
	};
}


// Core log macros
#define OL_CORE_TRACE(...)		::Soren::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define OL_CORE_INFO(...)		::Soren::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OL_CORE_WARN(...)		::Soren::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OL_CORE_ERROR(...)		::Soren::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OL_CORE_FATAL(...)		::Soren::Log::GetCoreLogger()->fatal(__VA_ARGS__)
		
// Client log macros
#define OL_TRACE(...)			::Soren::Log::GetClientLogger()->trace(__VA_ARGS__)
#define OL_INFO(...)			::Soren::Log::GetClientLogger()->info(__VA_ARGS__)
#define OL_WARN(...)			::Soren::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OL_ERROR(...)			::Soren::Log::GetClientLogger()->error(__VA_ARGS__)
#define OL_FATAL(...)			::Soren::Log::GetClientLogger()->fatal(__VA_ARGS__)

// Time log macros
#define OL_TIME_TRACE(...)			::Soren::Log::GetTimeLogger()->trace(__VA_ARGS__)
#define OL_TIME_INFO(...)			::Soren::Log::GetTimeLogger()->info(__VA_ARGS__)
#define OL_TIME_WARN(...)			::Soren::Log::GetTimeLogger()->warn(__VA_ARGS__)
#define OL_TIME_ERROR(...)			::Soren::Log::GetTimeLogger()->error(__VA_ARGS__)
#define OL_TIME_FATAL(...)			::Soren::Log::GetTimeLogger()->fatal(__VA_ARGS__)