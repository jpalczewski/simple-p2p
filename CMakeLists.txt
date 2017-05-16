cmake_minimum_required(VERSION 3.2.2)
project(simple_p2p)
set(CMAKE_CXX_STANDARD 14
        set(CMAKE_CXX_STANDARD_REQUIRED on)

        find_package(PkgConfig REQUIRED)



# OpenSSL
pkg_search_module(OPENSSL REQUIRED openssl)

if( OPENSSL_FOUND )
    message(STATUS "ski/simple-p2Using OpenSSL ${OPENSSL_VERSION}")

    include_directories(${OPENSSL_INCLUDE_DIRS})
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()



# Boost
find_package(Boost COMPONENTS filesystem serialization REQUIRED)
if( Boost_FOUND )
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})
endif()



# unit_test_framework
#Examples:
#add_executable(openssl_boost_playground main.cpp ${SOURCE_FILES})
#add_executable(obp_test ${TEST_FILES})
#target_link_libraries(openssl_boost_playground ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})
#target_link_libraries(obp_test ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})

add_subdirectory(src)
add_subdirectory(test)

enable_testing()
add_test(NAME testCli COMMAND test_cli)

#;set(SOURCE_CLI src/cli.cpp)
#add_executable(simple_cli ${SOURCE_CLI})
