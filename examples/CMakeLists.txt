include_directories(${simple_p2p_SOURCE_DIR}/src)

include(../src/Network/Network.cmake)
include(../src/Resources/Resources.cmake)
include(../src/common.cmake)

add_executable(server server.cpp ${COMMON} ${RESOURCES} ${NETWORK})

target_link_libraries(server Threads::Threads)
target_link_libraries(server ${OPENSSL_LIBRARIES})