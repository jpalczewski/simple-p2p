
add_library(NetworkCommandInterface NetworkCommandInterface.cpp)
#add_library(SimpleCommandInterface SimpleCommandInterface.cpp)
add_library(UserCommandsHandler UserCommandsHandler.cpp ResourceDownloadHandler.cpp)

target_link_libraries(UserCommandsHandler NetworkCommandInterface Messages FileManager)
target_link_libraries(NetworkCommandInterface ${Boost_SERIALIZATION_LIBRARY})