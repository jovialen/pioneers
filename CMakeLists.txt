cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)
project(pioneers VERSION 0.1.0 LANGUAGES C CXX)

# Dependencies

add_subdirectory("libs/asio-1.28.0")
add_subdirectory("libs/glfw")
add_subdirectory("libs/glm")
add_subdirectory("libs/spdlog")
add_subdirectory("libs/vk-bootstrap")

find_package(Vulkan REQUIRED)

# Pioneers

set(PIO_HEADERS
	"src/log.hpp"
	"src/network/client.hpp" "src/network/common.hpp" "src/network/connection.hpp"
	"src/network/queue_mt.hpp" "src/network/server.hpp" "src/network/message.hpp"
	"src/window/window.hpp" "src/window/common.hpp" "src/window/monitor.hpp"
	"src/render/context.hpp" "src/render/render_window.hpp" "src/render/swap_chain.hpp"
	"src/render/queue.hpp" "src/render/common.hpp")
set(PIO_SOURCES "src/main.cpp" "src/log.cpp"
	"src/network/client.cpp" "src/network/common.cpp" "src/network/connection.cpp"
	"src/network/queue_mt.cpp" "src/network/server.cpp" "src/network/message.cpp"
	"src/window/window.cpp" "src/window/common.cpp" "src/window/monitor.cpp"
	"src/render/context.cpp" "src/render/render_window.cpp" "src/render/swap_chain.cpp"
	"src/render/queue.cpp" "src/render/common.cpp")

add_executable(pioneers ${PIO_SOURCES} ${PIO_HEADERS})

set_target_properties(pioneers PROPERTIES
  FOLDER "Pioneers"
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS ON)

target_include_directories(pioneers PRIVATE "src")

target_link_libraries(pioneers PRIVATE asio)
target_link_libraries(pioneers PRIVATE glfw)
target_link_libraries(pioneers PRIVATE glm::glm)
target_link_libraries(pioneers PRIVATE spdlog::spdlog)
target_link_libraries(pioneers PRIVATE vk-bootstrap::vk-bootstrap)
target_link_libraries(pioneers PRIVATE Vulkan::Vulkan)
