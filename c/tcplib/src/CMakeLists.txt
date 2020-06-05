aux_source_directory(. SRC_LIST)
include_directories(../include)
link_libraries(pthread)
add_library(tcp_client ${SRC_LIST})
#add_executable(tcp ${SRC_LIST})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


