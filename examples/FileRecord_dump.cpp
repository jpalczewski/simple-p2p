//
// Created by erxyi on 22.05.2017.
//
#include "Files/FileManager.h"

int main()
{
    FileManager fm;
    fm.setWorkingDirectory("../../examples/test-resources");
    //FileRecord fr(0, boost::filesystem::unique_path(), {0});
    //FileRecord fri;

    auto authors = fm.getAllAuthors();
    auto files = fm.getAllFilesFromAuthor(authors[0]);

    FilePartRequest request;
    request.size = 4;
    request.offset = 0;
    request.fileHash = files[1] ;
    request.authorKey = authors[0];
    auto response = fm.getFilePart(request);
    std::cout << "Response:" << response.received;
    std::ofstream of("fr_dump");
    boost::archive::xml_oarchive oa(of);
    oa << BOOST_SERIALIZATION_NVP(fm);
    of.close();

    return 0;
}