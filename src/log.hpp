#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/spdlog.h>

void configure_logger();

#define PIO_TRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define PIO_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define PIO_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define PIO_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define PIO_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define PIO_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
