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
#include <ostream>

class Hash {
public:
    enum class InputTextType {Text, Hash, File, Invalid};

    Hash(const std::array<unsigned char, 16> &hash);
    Hash(const std::string &inputText, InputTextType inputType = InputTextType::Text);
    Hash(std::istream & inputStream);
    Hash(std::initializer_list<unsigned char> list);
    Hash(const boost::filesystem::path &p);
    Hash(const std::vector<unsigned char> & inputVector);



    std::string getString() const;
    std::array<unsigned char, 16> getArray() const;
    const std::vector<unsigned char> getVector() const;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(hash);
    }

    bool operator==(const Hash &rhs) const;

    bool operator!=(const Hash &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Hash &hash);

     // we can make this generic for any container [1]
    struct ContainerHasher {
        std::size_t operator()(Hash const& c) const {
            return boost::hash_range(c.hash.begin(), c.hash.end());
        }
    };

private:
    friend class boost::serialization::access;

    std::array<unsigned char, 16> hash;

    static const int BUFFER_SIZE = 65536;


    void getHashFromInputStream(std::istream &inputStream);
};


#endif //SIMPLE_P2P_HASH_H
