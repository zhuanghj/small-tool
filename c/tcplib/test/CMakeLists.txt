aux_source_directory(. SRC_LIST)
include_directories(../include)
link_libraries(tcp_client)
link_directories(${PROJECT_SOURCE_DIR}/bin)


#add_library(tcplib ${SRC_LIST})

add_executable(tcpTest ${SRC_LIST})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

