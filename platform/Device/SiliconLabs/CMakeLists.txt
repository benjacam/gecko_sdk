set(SRC_PATH "${CMAKE_CURRENT_LIST_DIR}/${SL_FAMILY_UPPER}/Source/")
add_library(gecko_device INTERFACE)
target_sources(gecko_device INTERFACE ${SRC_PATH}/system_${SL_FAMILY_LOWER}.c ${SRC_PATH}/startup_${SL_FAMILY_LOWER}.c)
target_include_directories(gecko_device INTERFACE ${SL_FAMILY_UPPER}/Include)