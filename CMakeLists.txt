cmake_minimum_required(VERSION 3.22)

project(xerxes-protocol C CXX ASM)
set(CMAKE_CXX_STANDARD 23)


set(xerxes-protocol_DIR ".")
find_package(xerxes-protocol REQUIRED)

include_directories(xerxes-protocol_INCLUDE_DIRS)

add_library(xerxes-protocol STATIC ${xerxes-protocol_SOURCES})

install(TARGETS xerxes-protocol DESTINATION lib/xerxes-protocol)
install(FILES ${xerxes-protocol_HEADERS} DESTINATION include/xerxes-protocol)