include(Network/Network.cmake)
include(Resources/Resources.cmake)
include(Messages/Messages.cmake)
include(Commands/Commands.cmake)
include(common.cmake)
add_subdirectory(Files)
add_subdirectory(CLI)
add_subdirectory(Crypto)
add_subdirectory(Resources)

add_executable(simple-p2p-daemon simple-p2p-daemon.cpp ${COMMON} ${RESOURCES} ${NETWORK} ${MESSAGES} ${COMMANDS})
target_link_libraries(simple-p2p-daemon Threads::Threads)
target_link_libraries(simple-p2p-daemon ${OPENSSL_LIBRARIES} ${Boost_SERIALIZATION_LIBRARY})

#add_executable(simple-p2p-daemon simple-p2p-daemon.cpp)

install(TARGETS simple-p2p-daemon RUNTIME DESTINATION ${simple_p2p_SOURCE_DIR}/release)
#install(TARGETS cli RUNTIME DESTINATION ${simple_p2p_SOURCE_DIR}/release)
