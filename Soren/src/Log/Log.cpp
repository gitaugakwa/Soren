#include "sorenpch.h"
#include "Log/Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Soren {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::shared_ptr<spdlog::logger> Log::s_TimeLogger;


	void Log::Init()
	{
		spdlog::set_pattern("%^{%T} %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("NEURAL");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);

		s_TimeLogger = spdlog::stdout_color_mt("TIME");
		s_TimeLogger->set_level(spdlog::level::trace);

	}

}