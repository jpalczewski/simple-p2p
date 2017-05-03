add_executable(cli cli.cpp Socket.cpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(cli Threads::Threads)