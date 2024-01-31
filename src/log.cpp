#include "log.hpp"

#include <vector>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

void configure_logger() {
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/pioneers.log", true));

	sinks[0]->set_level(spdlog::level::trace);
	sinks[1]->set_level(spdlog::level::info);

	auto logger = std::make_shared<spdlog::logger>("Pioneers", sinks.begin(), sinks.end());
	logger->set_level(spdlog::level::trace);
	logger->set_pattern("[%T.%e] [%^%-8l%$] %s@%!:%#: %v");

	spdlog::set_default_logger(logger);
}
