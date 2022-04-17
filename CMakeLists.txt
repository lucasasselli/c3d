cmake_minimum_required(VERSION 3.0)

project(graphics-sandbox)

set(CMAKE_BUILD_TYPE Debug)

find_package(glfw3 3.3 REQUIRED)

add_subdirectory(libs/gfx)
add_subdirectory(libs/glad)

file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB TARGET_FILES *.cpp)

add_executable(simple_rasterizer simple_rasterizer.c)
target_link_libraries(simple_rasterizer PRIVATE gfx m X11)

foreach(TARGET_FILE ${TARGET_FILES})
    get_filename_component(TARGET ${TARGET_FILE} NAME_WE)
    add_executable(${TARGET} ${TARGET_FILE} ${SOURCE_FILES})
    target_include_directories(${TARGET} PRIVATE include)
    target_link_libraries(${TARGET} PRIVATE glad glfw ${CMAKE_DL_LIBS})
endforeach()
