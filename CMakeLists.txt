cmake_minimum_required(VERSION 3.2.2)
project(simple_p2p)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
if(CYGWIN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcmodel=medium")
endif()
find_package(PkgConfig REQUIRED)


# OpenSSL
#find_package(openssl)
pkg_search_module(OPENSSL REQUIRED openssl)

if( OPENSSL_FOUND )
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
    message("${OPENSSL_LIBRARIES}")
    if("${OPENSSL_VERSION}" EQUAL "1.1.0e")
        message("It won't work with newer OpenSSL! I bet you have openssl-1.0 installed...")
        set(OPENSSL_INCLUDE_DIRS "/usr/include/openssl-1.0/")
        set(OPENSSL_LIBRARIES ":libssl.so.1.0.0" ":libcrypto.so.1.0.0")
    endif()
    include_directories(${OPENSSL_INCLUDE_DIRS})
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()



# Boost
find_package(Boost COMPONENTS system filesystem serialization REQUIRED)
if( Boost_FOUND )
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
endif()


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


#add_subdirectory(examples)
add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME testCli COMMAND test_cli)

#;set(SOURCE_CLI src/cli.cpp)
#add_executable(simple_cli ${SOURCE_CLI})
