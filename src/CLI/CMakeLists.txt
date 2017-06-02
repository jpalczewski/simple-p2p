find_package (Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(simple-p2p-cli simple-p2p-cli.cpp DaemonClient.cpp DaemonClientResponse.h ${COMMON} ${RESOURCES} ${NETWORK} ${MESSAGES} ${COMMANDS})
target_link_libraries(simple-p2p-cli Threads::Threads)
target_link_libraries(simple-p2p-cli ${Boost_LIBRARIES} ${Boost_SERIALIZATION_LIBRARY})
target_link_libraries(simple-p2p-cli ${OPENSSL_LIBRARIES} ${Boost_SERIALIZATION_LIBRARY} ${Boost_SYSTEM_LIBRARY})
target_link_libraries(simple-p2p-cli FileManager)

install(TARGETS simple-p2p-cli RUNTIME DESTINATION ${simple_p2p_SOURCE_DIR}/release)