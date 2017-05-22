//
// Created by erxyi on 22.05.2017.
//
#include "Files/FileManager.h"
#include <iostream>

int main()
{
    FileManager fm;
    HASH_ARRAY newFileHash = {1,2,3};
    std::string newFileName = "test.txt";
    fm.setWorkingDirectory("../../examples/test-resources");
    //FileRecord fr(0, boost::filesystem::unique_path(), {0});
    //FileRecord fri;

    auto authors = fm.getAllAuthors();
    auto files = fm.getAllFilesFromAuthor(authors[0]);

    FilePartRequest request(authors[0], files[1], 0, 4);

    FileCreateRequest createRequest(authors[0], newFileHash, newFileName, 300);
    FileSavePartRequest savePartRequest(authors[0], newFileHash, 0, 3);
    savePartRequest.bytes = {'l','o','l'};
    auto response = fm.getFilePart(request);
    std::cout << "Response:" << response.received;
    fm.createFile(createRequest);
    fm.saveFilePart(savePartRequest);
    std::ofstream of("fr_dump");
    boost::archive::xml_oarchive oa(of);
    oa << BOOST_SERIALIZATION_NVP(fm);
    of.close();

    return 0;
}