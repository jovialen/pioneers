cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)
project(asio VERSION 1.28.0 LANGUAGES C CXX)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_compile_definitions(asio INTERFACE "ASIO_STANDALONE")
