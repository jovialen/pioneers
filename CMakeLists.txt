cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)
project(pioneers VERSION 0.1.0 LANGUAGES C CXX)

# Dependencies

add_subdirectory("libs/spdlog")

# Pioneers

set(PIO_HEADERS "src/log.hpp")
set(PIO_SOURCES "src/main.cpp" "src/log.cpp")

add_executable(pioneers ${PIO_SOURCES} ${PIO_HEADERS})

set_target_properties(pioneers PROPERTIES
  FOLDER "Pioneers"
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)

target_include_directories(pioneers PRIVATE "src")

target_link_libraries(pioneers PRIVATE spdlog::spdlog)
