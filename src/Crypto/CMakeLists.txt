set(HASH Hash.cpp)

set (CMAKE_CXX_FLAGS "-mcmodel=large ${CMAKE_CXX_FLAGS}")

add_library(Hash HASH)
link_directories(Hash ${Boost_LIBRARY_DIRS})
target_link_libraries(Hash
        ${OPENSSL_LIBRARIES}
        ${Boost_SYSTEM_LIBRRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SERIALIZATION_LIBRARY}
        )