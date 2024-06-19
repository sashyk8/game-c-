cmake_minimum_required(VERSION 3.10)
project(pongAi)
add_executable(pongAi win32_platform.cpp)
#add_executable(game-test game.cpp game-test.cpp)
set(SOURCES win32_platform.cpp game.cpp platform_common.cpp renderer.cpp utils.cpp)
#include(CTest)
#enable_testing()
#add_test(NAME game-test COMMAND game-test)
#target_link_libraries(game-test doctest.h)
include_directories(.)


