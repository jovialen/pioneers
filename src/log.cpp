#include "log.hpp"

#include <vector>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/dist_sink.h>

namespace pio {
namespace log {

std::shared_ptr<spdlog::logger> g_client_logger;
std::shared_ptr<spdlog::logger> g_server_logger;
std::shared_ptr<spdlog::logger> g_gfx_logger;

static std::shared_ptr<spdlog::logger> configure_logger(spdlog::sink_ptr common_sink, const std::string &file, const std::string &name) {
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(file, true));
	sinks.push_back(common_sink);

	sinks[0]->set_level(spdlog::level::info);

	auto logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
	logger->set_level(spdlog::level::trace);
	logger->set_pattern("[%T.%e] [%^%-8l%$] [%n] %s@%!:%#: %v");

	return logger;
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

	g_client_logger = configure_logger(common, "logs/client.log", "Client");
	g_server_logger = configure_logger(common, "logs/server.log", "Server");
	g_gfx_logger = configure_logger(common, "logs/gfx.log", "Graphics");
}

void shutdown() {
	g_client_logger->flush();
	g_server_logger->flush();
	g_gfx_logger->flush();
}

}
}
