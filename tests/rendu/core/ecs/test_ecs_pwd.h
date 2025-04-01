/*
* Created by boil on 2023/3/2.
*/

#ifndef TEST_RENDU_CORE_ECS_TEST_ECS_PWD_H_
#define TEST_RENDU_CORE_ECS_TEST_ECS_PWD_H_

#include <unordered_set>
#include <core/ecs/sparse_set.h>
#include <core/ecs/entity.h>
#include <core/ecs/component.h>
#include <core/ecs/storage.h>

struct traits_based {};
template<>
struct rendu::component_traits<traits_based> {
  using type = traits_based;
  static constexpr auto in_place_delete = false;
  static constexpr auto page_size = 8u;
};

template<>
struct rendu::component_traits<std::unordered_set<char>> {
  static constexpr auto in_place_delete = true;
  static constexpr auto page_size = 4u;
};

struct empty_each_tag final {};

template<>
struct rendu::basic_storage<empty_each_tag, rendu::entity, std::allocator<empty_each_tag>> : rendu::basic_storage<void,
                                                                                                                  rendu::entity,
                                                                                                                  std::allocator<
                                                                                                                      void>> {
  basic_storage(const std::allocator<empty_each_tag> &) {}

  [[nodiscard]] iterable each() noexcept {
    return {internal::extended_storage_iterator{base_type::end()},
            internal::extended_storage_iterator{base_type::end()}};
  }
};

struct empty_type {};

struct stable_type {
  static constexpr auto in_place_delete = true;
  int value{};
};

struct non_default_constructible {
  non_default_constructible() = delete;

  non_default_constructible(int v)
      : value{v} {}

  int value{};
};

struct counter {
  int value{};
};

#endif //TEST_RENDU_CORE_ECS_TEST_ECS_PWD_H_
