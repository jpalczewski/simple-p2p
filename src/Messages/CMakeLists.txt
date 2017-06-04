set(MESSAGES ${simple_p2p_SOURCE_DIR}/src/Messages/BroadcastMessage.cpp
        ${simple_p2p_SOURCE_DIR}/src/Messages/ResourceRequestMessage.cpp
        ${simple_p2p_SOURCE_DIR}/src/Messages/SendResourceMessage.cpp
        )

add_library(Messages ${MESSAGES})
target_link_libraries(Messages Common)