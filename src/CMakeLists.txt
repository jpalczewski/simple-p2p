set(COMMON
        ${simple_p2p_SOURCE_DIR}/src/CryptoUtils.cpp
        ${simple_p2p_SOURCE_DIR}/src/Dispatcher.cpp
        ${simple_p2p_SOURCE_DIR}/src/Handler.cpp
        ${simple_p2p_SOURCE_DIR}/src/ConversionUtils.cpp
        )

add_library(Common ${COMMON})
target_link_libraries(Common ResourceManager Network Messages)

add_subdirectory(Files)
add_subdirectory(Network)
add_subdirectory(CLI)
add_subdirectory(Crypto)
add_subdirectory(Resources)
add_subdirectory(Commands)
add_subdirectory(Messages)


add_executable(cli cli.cpp ${COMMON} ${MESSAGES}) #${COMMANDS}

target_link_libraries(cli
        ${OPENSSL_LIBRARIES}
        ${Boost_SERIALIZATION_LIBRARY}
        Threads::Threads
        NetworkCommandInterface
        Network
        UserCommandsHandler
        ResourceManager
        Common
        )

add_executable(simple-p2p-daemon simple-p2p-daemon.cpp)

install(TARGETS simple-p2p-daemon RUNTIME DESTINATION ${simple_p2p_SOURCE_DIR}/release)