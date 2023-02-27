
cmake_minimum_required(VERSION 3.22)
set(PROJECT_NAME xerxes-protocol)

project(${PROJECT_NAME} C CXX ASM)
set(CMAKE_CXX_STANDARD 23)

include_directories(xerxes-protocol)

set(xerxes_files
    xerxes-protocol/Message.cpp
    xerxes-protocol/Network.cpp
    xerxes-protocol/Packet.cpp
    xerxes-protocol/Protocol.cpp
)

set(headers
    xerxes-protocol/Message.hpp
    xerxes-protocol/Network.hpp
    xerxes-protocol/Packet.hpp
    xerxes-protocol/Protocol.hpp
    xerxes-protocol/DeviceIds.h
    xerxes-protocol/MessageId.h
)

add_library(${PROJECT_NAME} STATIC ${xerxes_files})

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${headers} DESTINATION include/xerxes-protocol)