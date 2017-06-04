set (NETWORK IpAddress.cpp Socket.cpp UdpListener.cpp)

add_library(Network ${NETWORK})
target_link_libraries(Network)