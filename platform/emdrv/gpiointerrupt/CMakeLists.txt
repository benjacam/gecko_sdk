cmake_minimum_required(VERSION 3.12)
add_library(gpiointerrupt INTERFACE)
target_sources(gpiointerrupt INTERFACE $(CMAKE_CURRENT_LIST_DIR)/src/gpiointerrupt.c)
target_include_directories(gpiointerrupt $(CMAKE_CURRENT_LIST_DIR)/inc)
