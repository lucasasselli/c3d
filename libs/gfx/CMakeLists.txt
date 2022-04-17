cmake_minimum_required(VERSION 3.0)

project(gfx)

add_library(gfx src/gfx.c)
target_include_directories(gfx PUBLIC include/)