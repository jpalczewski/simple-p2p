include(Network/Network.cmake)
include(Resources/Resources.cmake)
include(Messages/Messages.cmake)
include(Commands/Commands.cmake)
include(common.cmake)
add_subdirectory(Files)
add_subdirectory(CLI)
add_subdirectory(Crypto)
add_subdirectory(Resources)
add_subdirectory(Messages)

add_executable(cli cli.cpp ${COMMON} ${RESOURCES} ${NETWORK} ${MESSAGES} ${COMMANDS})
target_link_libraries(cli Threads::Threads)
target_link_libraries(cli
        ${OPENSSL_LIBRARIES}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_SERIALIZATION_LIBRARY}
        Crypto
        FileManager
        Messages)

add_executable(simple-p2p-daemon simple-p2p-daemon.cpp)

install(TARGETS simple-p2p-daemon RUNTIME DESTINATION ${simple_p2p_SOURCE_DIR}/release)