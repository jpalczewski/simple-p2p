//
// Created by erxyi on 22.05.2017.
//

#ifndef SIMPLE_P2P_AUTHORKEYHASHER_H
#define SIMPLE_P2P_AUTHORKEYHASHER_H
#include <boost/functional/hash.hpp>

template <typename Container> // we can make this generic for any container [1]
struct AuthorKeyHasher {
    std::size_t operator()(Container const& c) const {
        return boost::hash_range(c.begin(), c.end());
    }
};
#endif //SIMPLE_P2P_AUTHORKEYHASHER_H
