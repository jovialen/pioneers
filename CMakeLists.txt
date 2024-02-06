cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)
project(pioneers VERSION 0.1.0 LANGUAGES C CXX)

# Dependencies

add_subdirectory("libs/spdlog")
add_subdirectory("libs/asio-1.28.0")

# Pioneers

set(PIO_HEADERS
	"src/log.hpp"
	"src/network/client.hpp" "src/network/common.hpp" "src/network/connection.hpp"
	"src/network/queue_mt.hpp" "src/network/server.hpp" "src/network/message.hpp")
set(PIO_SOURCES "src/main.cpp" "src/log.cpp"
	"src/network/client.cpp" "src/network/common.cpp" "src/network/connection.cpp"
	"src/network/queue_mt.cpp" "src/network/server.cpp" "src/network/message.cpp")

add_executable(pioneers ${PIO_SOURCES} ${PIO_HEADERS})

set_target_properties(pioneers PROPERTIES
  FOLDER "Pioneers"
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)

target_include_directories(pioneers PRIVATE "src")

target_link_libraries(pioneers PRIVATE asio)
target_link_libraries(pioneers PRIVATE spdlog::spdlog)
