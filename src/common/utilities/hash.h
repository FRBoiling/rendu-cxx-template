/*
* Created by boil on 2022/8/20.
*/

#ifndef RENDU_HASH_H_
#define RENDU_HASH_H_

#include <functional>
#include <utility>

namespace rendu
{
    template<typename T>
    inline void hash_combine(std::size_t& seed, T const& val)
    {
        seed ^= std::hash<T>()(val) + 0x9E3779B9 + (seed << 6) + (seed >> 2);
    }
}

 //! Hash implementation for std::pair to allow using pairs in unordered_set or as key for unordered_map
 //! Individual types used in pair must be hashable by std::hash
namespace std
{
    template<class K, class V>
    struct hash<std::pair<K, V>>
    {
    public:
        size_t operator()(std::pair<K, V> const& p) const
        {
            size_t hashVal = 0;
            rendu::hash_combine(hashVal, p.first);
            rendu::hash_combine(hashVal, p.second);
            return hashVal;
        }
    };
}

#endif // RENDU_HASH_H_
