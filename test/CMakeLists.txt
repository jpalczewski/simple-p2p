find_package (Boost COMPONENTS unit_test_framework REQUIRED)

#set (CMAKE_CXX_FLAGS "-mcmodel=large ${CMAKE_CXX_FLAGS}")

if (Boost_UNIT_TEST_FRAMEWORK_FOUND )
    message(STATUS "Boost UTF found!")
    message(STATUS "Boost_UNIT_TEST_FRAMEWORK_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_UNIT_TEST_FRAMEWORK_LIBRARIES: ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES}")
    # include_directories(${Boost_INCLUDE_DIRS}) - it isn't necessary
endif()

include_directories(${simple_p2p_SOURCE_DIR}/src)
add_definitions (-DBOOST_TEST_DYN_LINK)
set(BOOST_TEST
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )

add_executable(test_FileManager test_FileManager.cpp)
add_executable(test_Crypto          test_Crypto.cpp)
add_executable(test_ResourceFile    test_ResourceFile.cpp)
add_executable(test_ResourceManager test_ResourceManager.cpp)
add_executable(test_Messages test_Messages.cpp)

target_link_libraries(test_FileManager
        ${BOOST_TEST}
        FileManager
        )

target_link_libraries(test_Crypto
        ${BOOST_TEST}
        Crypto)

target_link_libraries(test_ResourceFile
        ${BOOST_TEST}
        Crypto
        FileManager
        ResourceManager)

target_link_libraries(test_Messages
        ${BOOST_TEST}
        Crypto
        FileManager
        ResourceManager
        Messages)

target_link_libraries(test_ResourceManager
        ${BOOST_TEST}
        ResourceManager)