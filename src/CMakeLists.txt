include(Network/Network.cmake)
include(Resources/Resources.cmake)
include(Messages/Messages.cmake)
include(Commands/Commands.cmake)
include(common.cmake)
add_subdirectory(Files)
add_executable(cli cli.cpp ${COMMON} ${RESOURCES} ${NETWORK} ${MESSAGES} ${COMMANDS})

target_link_libraries(cli Threads::Threads)

target_link_libraries(cli ${OPENSSL_LIBRARIES})