//
// Created by erxyi on 22.05.2017.
//
#include "Files/FileRecord.h"

int main()
{
    FileRecord fr(0, boost::filesystem::unique_path(), {0});
    FileRecord fri;
    std::ofstream of("fr_dump");
    boost::archive::xml_oarchive oa(of);
    oa << BOOST_SERIALIZATION_NVP(fr);
    of.close();

    std::ifstream ifs("fr_dump");
    boost::archive::xml_iarchive ia(ifs);
    ia >> BOOST_SERIALIZATION_NVP(fri);
    return 0;
}