find_package (Boost COMPONENTS unit_test_framework REQUIRED)
if (Boost_UNIT_TEST_FRAMEWORK_FOUND )
    message(STATUS "Boost UTF found!")
    message(STATUS "Boost_UNIT_TEST_FRAMEWORK_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_UNIT_TEST_FRAMEWORK_LIBRARIES: ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES}")
    # include_directories(${Boost_INCLUDE_DIRS}) - it isn't necessary
endif()

include_directories(${simple_p2p_SOURCE_DIR}/src)
add_definitions (-DBOOST_TEST_DYN_LINK)

add_executable(test_cli test_cli.cpp)

target_link_libraries(test_cli
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )
