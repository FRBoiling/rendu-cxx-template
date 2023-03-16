/*
* Created by boil on 2023/2/26.
*/

#ifndef RENDU_CORE_META_POINTER_H_
#define RENDU_CORE_META_POINTER_H_

#include <memory>
#include <type_traits>
#include "type_traits.h"

namespace rendu {

/**
 * @brief Makes plain pointers pointer-like types for the meta system.
 * @tparam Type Element type.
 */
template<typename Type>
struct is_meta_pointer_like<Type *>
    : std::true_type {};

/**
 * @brief Partial specialization used to reject pointers to arrays.
 * @tparam Type Type of elements of the array.
 * @tparam N Number of elements of the array.
 */
template<typename Type, std::size_t N>
struct is_meta_pointer_like<Type (*)[N]>
    : std::false_type {};

/**
 * @brief Makes `std::shared_ptr`s of any type pointer-like types for the meta
 * system.
 * @tparam Type Element type.
 */
template<typename Type>
struct is_meta_pointer_like<std::shared_ptr<Type>>
    : std::true_type {};

/**
 * @brief Makes `std::unique_ptr`s of any type pointer-like types for the meta
 * system.
 * @tparam Type Element type.
 * @tparam Args Other arguments.
 */
template<typename Type, typename... Args>
struct is_meta_pointer_like<std::unique_ptr<Type, Args...>>
    : std::true_type {};

} // namespace rendu

#endif //RENDU_CORE_META_POINTER_H_
