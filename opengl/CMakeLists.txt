cmake_minimum_required(VERSION 3.0)

project(graphics-sandbox-opengl)

set(CMAKE_BUILD_TYPE Debug)

find_package(glfw3 3.3 REQUIRED)

add_subdirectory(libs/glad)

file(GLOB SOURCE_FILES src/*.cpp)

add_executable(hello_world hello_world.cpp ${SOURCE_FILES})

target_include_directories(hello_world PRIVATE include)
target_link_libraries(hello_world PRIVATE glad glfw ${CMAKE_DL_LIBS})