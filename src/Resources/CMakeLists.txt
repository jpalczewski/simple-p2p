set(RESOURCEMANAGER LocalResourceInfo.cpp NetworkResourceInfo.cpp Resource.cpp ResourceManager.cpp Resource.cpp ../ConversionUtils.cpp)


add_library(ResourceManager ${RESOURCEMANAGER})


link_directories(ResourceManager ${Boost_LIBRARY_DIRS})
target_link_libraries(ResourceManager
        ${OPENSSL_LIBRARIES}
        Network
        Crypto
        Threads::Threads
        )