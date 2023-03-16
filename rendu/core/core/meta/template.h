/*
* Created by boil on 2023/2/26.
*/

#ifndef RENDU_CORE_META_TEMPLATE_H_
#define RENDU_CORE_META_TEMPLATE_H_

#include "type_traits.h"

namespace rendu {

/*! @brief Utility class to disambiguate class templates. */
template<template<typename...> class>
struct meta_class_template_tag {};

/**
 * @brief General purpose traits class for generating meta template information.
 * @tparam Clazz Type of class template.
 * @tparam Args Types of template arguments.
 */
template<template<typename...> class Clazz, typename... Args>
struct meta_template_traits<Clazz<Args...>> {
  /*! @brief Wrapped class template. */
  using class_type = meta_class_template_tag<Clazz>;
  /*! @brief List of template arguments. */
  using args_type = type_list<Args...>;
};

} // namespace rendu
#endif //RENDU_CORE_META_TEMPLATE_H_
