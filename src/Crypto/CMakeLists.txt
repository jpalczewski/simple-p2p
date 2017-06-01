add_library(Crypto Hash.cpp AuthorKey.cpp)
link_directories(Crypto ${Boost_LIBRARY_DIRS})
target_link_libraries(Crypto
        ${OPENSSL_LIBRARIES}
        ${Boost_SYSTEM_LIBRRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SERIALIZATION_LIBRARY}
        )