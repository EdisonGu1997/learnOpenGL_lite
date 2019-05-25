cmake_minimum_required(VERSION 3.11)
project(LearnOpenGL-lite)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
include_directories(
    ${PROJECT_SOURCE_DIR}/include
)
link_directories(lib)
add_subdirectory(src ${PROJECT_SOURCE_DIR}/bin)
