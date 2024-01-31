#include <iostream>
#include <vector>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

void configure_logger() {
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/pioneers.log", true));
	auto logger = std::make_shared<spdlog::logger>("Pioneers", sinks.begin(), sinks.end());
	spdlog::set_default_logger(logger);
}

int main(int argc, char *argv[]) {
	configure_logger();
	spdlog::info("Hello, world!");
	return 0;
}
