/*
* Created by boil on 2023/2/17.
*/

#ifndef RENDU_CORE_CONTAINER_FWD_H_
#define RENDU_CORE_CONTAINER_FWD_H_

#include <functional>
#include <memory>

namespace rendu {

template<
    typename Key,
    typename Type,
    typename = std::hash<Key>,
    typename = std::equal_to<Key>,
    typename = std::allocator<std::pair<const Key, Type>>>
class dense_map;

template<
    typename Type,
    typename = std::hash<Type>,
    typename = std::equal_to<Type>,
    typename = std::allocator<Type>>
class dense_set;

} // namespace rendu

#endif //RENDU_CORE_CONTAINER_FWD_H_
