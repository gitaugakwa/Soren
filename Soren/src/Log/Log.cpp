#include "sorenpch.h"
#include "Log/Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Soren {

	std::shared_ptr<spdlog::logger> Log::sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::sClientLogger;
	std::shared_ptr<spdlog::logger> Log::sTimeLogger;


	void Log::Init()
	{
		spdlog::set_pattern("%^{%T} %n: %v%$");
		sCoreLogger = spdlog::stdout_color_mt("NEURAL");
		sCoreLogger->set_level(spdlog::level::trace);

		sClientLogger = spdlog::stdout_color_mt("APP");
		sClientLogger->set_level(spdlog::level::trace);

		sTimeLogger = spdlog::stdout_color_mt("TIME");
		sTimeLogger->set_level(spdlog::level::trace);

	}

}