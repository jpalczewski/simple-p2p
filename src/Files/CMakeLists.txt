set(MD5UTILS MD5Utils.cpp)


set(FILEMESSAGES Requests/FilePartRequest.h Requests/FilePartRequest.h Requests/FilePartResponse.h Requests/GenericFileRequest.h Requests/FileCreateRequest.h Requests/FileSavePartRequest.h Requests/AddFileRequest.h)
set(FILEMANAGER FileManagerTypes.h FileManager.cpp FileRecord.cpp AuthorKeyHasher.h ${FILEMESSAGES})

set (CMAKE_CXX_FLAGS "-mcmodel=large ${CMAKE_CXX_FLAGS}")

add_library(FileManager ${FILEMANAGER} ${MD5UTILS})

link_directories(FileManager ${Boost_LIBRARY_DIRS})

target_link_libraries(FileManager
        ${OPENSSL_LIBRARIES}
        ${Boost_SYSTEM_LIBRRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SERIALIZATION_LIBRARY}
        Hash
        )