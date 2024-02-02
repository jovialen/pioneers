#include "log.hpp"

#include <vector>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/dist_sink.h>

namespace pio {
namespace log {

std::shared_ptr<spdlog::logger> g_client_logger;
std::shared_ptr<spdlog::logger> g_server_logger;

static void configure_client_logger(spdlog::sink_ptr common_sink) {
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/client.log", true));
	sinks.push_back(common_sink);

	sinks[0]->set_level(spdlog::level::info);

	g_client_logger = std::make_shared<spdlog::logger>("Client", sinks.begin(), sinks.end());
	g_client_logger->set_level(spdlog::level::trace);
	g_client_logger->set_pattern("[%T.%e] [%^%-8l%$] [%n] %s@%!:%#: %v");
}

static void configure_server_logger(spdlog::sink_ptr common_sink) {
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/server.log", true));
	sinks.push_back(common_sink);

	sinks[0]->set_level(spdlog::level::info);

	g_server_logger = std::make_shared<spdlog::logger>("Server", sinks.begin(), sinks.end());
	g_server_logger->set_level(spdlog::level::trace);
	g_server_logger->set_pattern("[%T.%e] [%^%-8l%$] [%n] %s@%!:%#: %v");
}

void init() {
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/errors.log", true));
	sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/pioneers.log", true));

	sinks[0]->set_level(spdlog::level::trace);
	sinks[1]->set_level(spdlog::level::err);
	sinks[2]->set_level(spdlog::level::info);

	auto common = std::make_shared<spdlog::sinks::dist_sink_mt>(sinks);
	common->set_level(spdlog::level::trace);

	configure_client_logger(common);
	configure_server_logger(common);
}

}
}
