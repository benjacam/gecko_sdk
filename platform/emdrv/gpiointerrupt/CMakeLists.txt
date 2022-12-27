add_library(gecko_gpiointerrupt INTERFACE)
target_sources(gecko_gpiointerrupt INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/gpiointerrupt.c)
target_include_directories(gecko_gpiointerrupt INTERFACE ${CMAKE_CURRENT_LIST_DIR}/inc)
