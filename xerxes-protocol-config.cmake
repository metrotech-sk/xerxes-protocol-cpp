set(PREFIX ${CMAKE_CURRENT_LIST_DIR}/xerxes-protocol)
set(xerxes-protocol_VERSION 1.4.0)

set(xerxes-protocol_SOURCES
${PREFIX}/Message.cpp
${PREFIX}/Network.cpp
${PREFIX}/Packet.cpp
${PREFIX}/Protocol.cpp
)

set(xerxes-protocol_HEADERS
${PREFIX}/Message.hpp
${PREFIX}/Network.hpp
${PREFIX}/Packet.hpp
${PREFIX}/Protocol.hpp
${PREFIX}/DeviceIds.h
${PREFIX}/MessageId.h
)

set(xerxes-protocol_INCLUDE_DIRS ${PREFIX}/)