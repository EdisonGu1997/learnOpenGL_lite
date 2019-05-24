set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(main main.cpp glad.c shader.cpp)
target_link_libraries(main glfw3)