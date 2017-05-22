include_directories(${simple_p2p_SOURCE_DIR}/src)

include(../src/Network/Network.cmake)
include(../src/Resources/Resources.cmake)
include(../src/common.cmake)

add_executable(server server.cpp ${COMMON} ${RESOURCES} ${NETWORK})
target_link_libraries(server Threads::Threads)
target_link_libraries(server ${OPENSSL_LIBRARIES})


add_executable(FileRecord_dump FileRecord_dump.cpp)
target_link_libraries(FileRecord_dump
        FileManager
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_SERIALIZATION_LIBRARY}
        )