#pragma once
#pragma warning( disable : 4189 ) // Disable Warning C4189: 'zero': local variable is initialized but not referenced

#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Soren {
	class SOREN_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() {return sCoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {return sClientLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetTimeLogger() {return sTimeLogger; }

	private:
		static std::shared_ptr<spdlog::logger> sCoreLogger;
		static std::shared_ptr<spdlog::logger> sClientLogger;
		static std::shared_ptr<spdlog::logger> sTimeLogger;
	};
}


// Core log macros
#define SOREN_CORE_TRACE(...)		::Soren::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SOREN_CORE_INFO(...)		::Soren::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SOREN_CORE_WARN(...)		::Soren::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SOREN_CORE_ERROR(...)		::Soren::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SOREN_CORE_FATAL(...)		::Soren::Log::GetCoreLogger()->fatal(__VA_ARGS__)
		
// Client log macros
#define SOREN_TRACE(...)			::Soren::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SOREN_INFO(...)			::Soren::Log::GetClientLogger()->info(__VA_ARGS__)
#define SOREN_WARN(...)			::Soren::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SOREN_ERROR(...)			::Soren::Log::GetClientLogger()->error(__VA_ARGS__)
#define SOREN_FATAL(...)			::Soren::Log::GetClientLogger()->fatal(__VA_ARGS__)

// Time log macros
#define SOREN_TIME_TRACE(...)			::Soren::Log::GetTimeLogger()->trace(__VA_ARGS__)
#define SOREN_TIME_INFO(...)			::Soren::Log::GetTimeLogger()->info(__VA_ARGS__)
#define SOREN_TIME_WARN(...)			::Soren::Log::GetTimeLogger()->warn(__VA_ARGS__)
#define SOREN_TIME_ERROR(...)			::Soren::Log::GetTimeLogger()->error(__VA_ARGS__)
#define SOREN_TIME_FATAL(...)			::Soren::Log::GetTimeLogger()->fatal(__VA_ARGS__)

#pragma warning( default : 4189 ) // Reenable Warning C4189: 'zero': local variable is initialized but not referenced
