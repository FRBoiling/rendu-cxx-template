/*
* Created by boil on 2023/2/12.
*/

#ifndef RENDU_CORE_ECS_COMPONENT_H_
#define RENDU_CORE_ECS_COMPONENT_H_

#include "base/type_traits.h"

namespace rendu {

namespace internal {

template<typename Type, typename = void>
struct in_place_delete : std::bool_constant<!(std::is_move_constructible_v<Type> &&
    std::is_move_assignable_v<Type>)> {
};

template<>
struct in_place_delete<void> : std::false_type {
};

template<typename Type>
struct in_place_delete<Type, std::enable_if_t<Type::in_place_delete>>
    : std::true_type {
};

template<typename Type, typename = void>
struct page_size : std::integral_constant<std::size_t, !std::is_empty_v<RD_ETO_TYPE(Type)> * RD_PACKED_PAGE> {
};

template<>
struct page_size<void> : std::integral_constant<std::size_t, 0u> {
};

template<typename Type>
struct page_size<Type, std::enable_if_t<std::is_convertible_v<decltype(Type::page_size), std::size_t>>>
    : std::integral_constant<std::size_t, Type::page_size> {
};

} // namespace internal

/**
 * @brief 访问组件各种属性的常用方法.
 * @tparam Type Type of component.
 */
template<typename Type, typename=void>
struct component_traits {
  static_assert(std::is_same_v<std::decay_t<Type>, Type>, "Unsupported type");

  /*! @brief Component type. */
  using type = Type;

  /*! @brief Pointer stability, default is `false`. */
  static constexpr bool in_place_delete = internal::in_place_delete<Type>::value;
  /*! @brief Page size, default is `RD_PACKED_PAGE` for non-empty types. */
  static constexpr std::size_t page_size = internal::page_size<Type>::value;
};

}//namespace rendu

#endif //RENDU_CORE_ECS_COMPONENT_H_
