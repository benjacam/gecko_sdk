add_library(gecko_common INTERFACE)
target_include_directories(gecko_common INTERFACE ${CMAKE_CURRENT_LIST_DIR}/inc ${CMAKE_CURRENT_LIST_DIR}/toolchain/inc ${CMAKE_CURRENT_LIST_DIR}/toolchain/config/small)
target_sources(gecko_common INTERFACE ${CMAKE_CURRENT_LIST_DIR}/toolchain/src/sl_memory.c)
