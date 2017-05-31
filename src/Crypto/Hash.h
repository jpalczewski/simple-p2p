//
// Created by erxyi on 31.05.2017.
//

#ifndef SIMPLE_P2P_HASH_H
#define SIMPLE_P2P_HASH_H

#include <array>
#include <string>
#include <openssl/md5.h>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class Hash {
public:
    enum class InputTextType {Text, Hash};

    Hash(const std::array<unsigned char, 16> &hash);
    Hash(const std::string &inputText, InputTextType inputType = InputTextType::Text);
    Hash(std::istream & inputStream);
    Hash(boost::filesystem::path &p);

    std::string getString() const;
    std::array<unsigned char, 16> getArray() const;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(hash);
    }

private:
    friend class boost::serialization::access;

    std::array<unsigned char, 16> hash;

    static const int BUFFER_SIZE = 65536;


    void getHashFromInputStream(std::istream &inputStream);
};


#endif //SIMPLE_P2P_HASH_H
