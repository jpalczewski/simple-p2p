include(Network/Network.cmake)
include(Resources/Resources.cmake)
include(common.cmake)

add_executable(cli cli.cpp ${COMMON} ${RESOURCES} ${NETWORK})

target_link_libraries(cli Threads::Threads)

target_link_libraries(cli ${OPENSSL_LIBRARIES})