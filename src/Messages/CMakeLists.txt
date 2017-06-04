set(MESSAGES ${simple_p2p_SOURCE_DIR}/src/Messages/BroadcastMessage.cpp
        ${simple_p2p_SOURCE_DIR}/src/Messages/ResourceRequestMessage.cpp
        ${simple_p2p_SOURCE_DIR}/src/Messages/SendResourceMessage.cpp
        ${simple_p2p_SOURCE_DIR}/src/ConversionUtils.cpp
        )

add_library(Messages ${MESSAGES})


link_directories(Messages ${Boost_LIBRARY_DIRS})
target_link_libraries(Messages
        ${OPENSSL_LIBRARIES}
        )