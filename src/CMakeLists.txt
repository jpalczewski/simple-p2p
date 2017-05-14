add_executable(cli cli.cpp Network/Socket.cpp Dispatcher.cpp Handler.cpp CryptoUtils.cpp examples.cpp Resources/Resource.cpp Resources/ResourceManager.cpp Resources/ResourceInfo.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(cli Threads::Threads)

target_link_libraries(cli ${OPENSSL_LIBRARIES})