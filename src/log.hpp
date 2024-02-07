#pragma once

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <spdlog/spdlog.h>

namespace pio {
namespace log {

extern std::shared_ptr<spdlog::logger> g_client_logger;
extern std::shared_ptr<spdlog::logger> g_server_logger;
extern std::shared_ptr<spdlog::logger> g_gfx_logger;

void init();

}
}


#define CLIENT_TRACE(...) SPDLOG_LOGGER_TRACE(::pio::log::g_client_logger, __VA_ARGS__)
#define CLIENT_DEBUG(...) SPDLOG_LOGGER_DEBUG(::pio::log::g_client_logger, __VA_ARGS__)
#define CLIENT_INFO(...) SPDLOG_LOGGER_INFO(::pio::log::g_client_logger, __VA_ARGS__)
#define CLIENT_WARN(...) SPDLOG_LOGGER_WARN(::pio::log::g_client_logger, __VA_ARGS__)
#define CLIENT_ERROR(...) SPDLOG_LOGGER_ERROR(::pio::log::g_client_logger, __VA_ARGS__)
#define CLIENT_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::pio::log::g_client_logger, __VA_ARGS__)

#define SERVER_TRACE(...) SPDLOG_LOGGER_TRACE(::pio::log::g_server_logger, __VA_ARGS__)
#define SERVER_DEBUG(...) SPDLOG_LOGGER_DEBUG(::pio::log::g_server_logger, __VA_ARGS__)
#define SERVER_INFO(...) SPDLOG_LOGGER_INFO(::pio::log::g_server_logger, __VA_ARGS__)
#define SERVER_WARN(...) SPDLOG_LOGGER_WARN(::pio::log::g_server_logger, __VA_ARGS__)
#define SERVER_ERROR(...) SPDLOG_LOGGER_ERROR(::pio::log::g_server_logger, __VA_ARGS__)
#define SERVER_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::pio::log::g_server_logger, __VA_ARGS__)

#define GFX_TRACE(...) SPDLOG_LOGGER_TRACE(::pio::log::g_gfx_logger, __VA_ARGS__)
#define GFX_DEBUG(...) SPDLOG_LOGGER_DEBUG(::pio::log::g_gfx_logger, __VA_ARGS__)
#define GFX_INFO(...) SPDLOG_LOGGER_INFO(::pio::log::g_gfx_logger, __VA_ARGS__)
#define GFX_WARN(...) SPDLOG_LOGGER_WARN(::pio::log::g_gfx_logger, __VA_ARGS__)
#define GFX_ERROR(...) SPDLOG_LOGGER_ERROR(::pio::log::g_gfx_logger, __VA_ARGS__)
#define GFX_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::pio::log::g_gfx_logger, __VA_ARGS__)

