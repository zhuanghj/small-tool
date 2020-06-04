aux_source_directory(. SRC_LIST)
include_directories("/usr/local/apr/include/apr-1")
link_directories(/usr/loca/apr/lib)
link_libraries(apr)
add_library(tcplib ${SRC_LIST})

