/*
* Created by boil on 2023/2/15.
*/

#ifndef RENDU_CORE_ECS_ENTITY_H_
#define RENDU_CORE_ECS_ENTITY_H_

#include <bit>
#include "base/type_traits.h"

namespace rendu {

namespace internal {

template<typename Type>
static constexpr int popcount(Type value) noexcept {
//      return value ? (int(value & 1) + popcount(value >> 1)) : 0;
  return std::popcount(value);// C++20
}

template<typename, typename = void>
struct entity_traits;

template<typename Type>
struct entity_traits<Type, std::enable_if_t<std::is_enum_v<Type>>>
    : entity_traits<std::underlying_type_t<Type>> {
  using value_type = Type;
};

template<typename Type>
struct entity_traits<Type, std::enable_if_t<std::is_class_v<Type>>>
    : entity_traits<typename Type::entity_type> {
  using value_type = Type;
};

template<>
struct entity_traits<std::uint32_t> {
  using value_type = std::uint32_t;

  using entity_type = std::uint32_t;
  using version_type = std::uint16_t;

  static constexpr entity_type entity_mask = 0xFFFFF;
  static constexpr entity_type version_mask = 0xFFF;
};

template<>
struct entity_traits<std::uint64_t> {
  using value_type = std::uint64_t;

  using entity_type = std::uint64_t;
  using version_type = std::uint32_t;

  static constexpr entity_type entity_mask = 0xFFFFFFFF;
  static constexpr entity_type version_mask = 0xFFFFFFFF;
};

} // namespace internal

/**
 * @brief 常见基本实体特征实现。
 * @tparam Traits 要使用的实际实体特征。
 */
template<typename Traits>
class basic_entity_traits {
  static constexpr auto length = internal::popcount(Traits::entity_mask);

  static_assert(Traits::entity_mask && ((typename Traits::entity_type{1} << length) == (Traits::entity_mask + 1)),
                "Invalid entity mask");
  static_assert(
      (typename Traits::entity_type{1} << internal::popcount(Traits::version_mask)) == (Traits::version_mask + 1),
      "Invalid version mask");

 public:
  /*! @brief Value type. */
  using value_type = typename Traits::value_type;
  /*! @brief Underlying entity type. */
  using entity_type = typename Traits::entity_type;
  /*! @brief Underlying version type. */
  using version_type = typename Traits::version_type;

  /*! @brief Entity mask size. */
  static constexpr entity_type entity_mask = Traits::entity_mask;
  /*! @brief Version mask size */
  static constexpr entity_type version_mask = Traits::version_mask;

  /**
   * @brief 将实体转换为其基础类型.
   * @param value 要转换的值.
   * @return 给定值的整数表示
   */
  [[nodiscard]] static constexpr entity_type to_integral(const value_type value) noexcept {
    return static_cast<entity_type>(value);
  }

  /**
   * @brief 转换为基础类型后返回entity.
   * @param value 要转换的值.
   * @return entity的整数表示.
   */
  [[nodiscard]] static constexpr entity_type to_entity(const value_type value) noexcept {
    return (to_integral(value) & entity_mask);
  }

  /**
   * @brief 转换为基础类型后返回version.
   * @param value 要转换的值.
   * @return  version的整数表示.
   */
  [[nodiscard]] static constexpr version_type to_version(const value_type value) noexcept {
    return (to_integral(value) >> length);
  }

  /**
 * @brief Returns the successor of a given identifier.
 * @param value The identifier of which to return the successor.
 * @return The successor of the given identifier.
 */
  [[nodiscard]] static constexpr value_type next(const value_type value) noexcept {
    const auto vers = to_version(value) + 1;
    return construct(to_entity(value), static_cast<version_type>(vers + (vers == version_mask)));
  }

  /**
   * @brief 通过entity和version构造标识符.
   * 如果未提供version，则返回逻辑删除
   * 如果未提供entity，则返回空标识符
   *
   * @param entity The entity part of the identifier.
   * @param version The version part of the identifier.
   * @return A properly constructed identifier.
   */
  [[nodiscard]] static constexpr value_type construct(const entity_type entity, const version_type version) noexcept {
    return value_type{(entity & entity_mask) | (static_cast<entity_type>(version) << length)};
  }

  /**
   * @brief 合并标识符.
   *
   * 返回的标识符是第一个元素的副本，但其version除外，该version取自第二个元素。
   *
   * @param lhs The identifier from which to take the entity part.
   * @param rhs The identifier from which to take the version part.
   * @return A properly constructed identifier.
   */
  [[nodiscard]] static constexpr value_type combine(const entity_type lhs, const entity_type rhs) noexcept {
    constexpr auto mask = (version_mask << length);
    return value_type{(lhs & entity_mask) | (rhs & mask)};
  }
};

/**
 * @brief Entity traits.
 * @tparam Type Type of identifier.
 */
template<typename Type>
struct entity_traits : basic_entity_traits<internal::entity_traits<Type>> {
  /*! @brief Base type. */
  using base_type = basic_entity_traits<internal::entity_traits<Type>>;
  /*! @brief Page size, default is `RD_SPARSE_PAGE`. */
  static constexpr std::size_t page_size = RD_SPARSE_PAGE;
};

/**
 * @copydoc entity_traits<Entity>::to_integral
 * @tparam Entity The value type.
 */
template<typename Entity>
[[nodiscard]] constexpr typename entity_traits<Entity>::entity_type to_integral(const Entity value) noexcept {
  return entity_traits<Entity>::to_integral(value);
}

/**
 * @copydoc entity_traits<Entity>::to_entity
 * @tparam Entity The value type.
 */
template<typename Entity>
[[nodiscard]] constexpr typename entity_traits<Entity>::entity_type to_entity(const Entity value) noexcept {
  return entity_traits<Entity>::to_entity(value);
}

/**
 * @copydoc entity_traits<Entity>::to_version
 * @tparam Entity The value type.
 */
template<typename Entity>
[[nodiscard]] constexpr typename entity_traits<Entity>::version_type to_version(const Entity value) noexcept {
  return entity_traits<Entity>::to_version(value);
}

/*! @brief Null object for all identifiers.  */
struct entity_null_t {
  /**
   * @brief Converts the null object to identifiers of any type.
   * @tparam Entity Type of identifier.
   * @return The null representation for the given type.
   */
  template<typename Entity>
  [[nodiscard]] constexpr operator Entity() const noexcept {
    using traits_type = entity_traits<Entity>;
    constexpr auto value = traits_type::construct(traits_type::entity_mask, traits_type::version_mask);
    return value;
  }

  /**
   * @brief Compares two null objects.
   * @param other A null object.
   * @return True in all cases.
   */
  [[nodiscard]] constexpr bool operator==([[maybe_unused]] const entity_null_t other) const noexcept {
    return true;
  }

  /**
   * @brief Compares two null objects.
   * @param other A null object.
   * @return False in all cases.
   */
  [[nodiscard]] constexpr bool operator!=([[maybe_unused]] const entity_null_t other) const noexcept {
    return false;
  }

  /**
   * @brief Compares a null object and an identifier of any type.
   * @tparam Entity Type of identifier.
   * @param entity Identifier with which to compare.
   * @return False if the two elements differ, true otherwise.
   */
  template<typename Entity>
  [[nodiscard]] constexpr bool operator==(const Entity entity) const noexcept {
    using traits_type = entity_traits<Entity>;
    return traits_type::to_entity(entity) == traits_type::to_entity(*this);
  }

  /**
   * @brief Compares a null object and an identifier of any type.
   * @tparam Entity Type of identifier.
   * @param entity Identifier with which to compare.
   * @return True if the two elements differ, false otherwise.
   */
  template<typename Entity>
  [[nodiscard]] constexpr bool operator!=(const Entity entity) const noexcept {
    return !(entity == *this);
  }

};

/**
* @brief Compares a null object and an identifier of any type.
* @tparam Entity Type of identifier.
* @param entity Identifier with which to compare.
* @param other A null object yet to be converted.
* @return False if the two elements differ, true otherwise.
*/
template<typename Entity>
[[nodiscard]] constexpr bool operator==(const Entity entity, const entity_null_t other) noexcept {
  return other.operator==(entity);
}

/**
 * @brief Compares a null object and an identifier of any type.
 * @tparam Entity Type of identifier.
 * @param entity Identifier with which to compare.
 * @param other A null object yet to be converted.
 * @return True if the two elements differ, false otherwise.
 */
template<typename Entity>
[[nodiscard]] constexpr bool operator!=(const Entity entity, const entity_null_t other) noexcept {
  return !(other == entity);
}

/**
 * @brief Compile-time constant for null entities.
 *
 * There exist implicit conversions from this variable to identifiers of any
 * allowed type. Similarly, there exist comparison operators between the null
 * entity and any other identifier.
 */
inline constexpr entity_null_t null{};

/*! @brief 所有标识符的逻辑删除对象.  */
struct entity_tombstone_t {
  /**
   * @brief Converts the tombstone object to identifiers of any type.
   * @tparam Entity Type of identifier.
   * @return The tombstone representation for the given type.
   */
  template<typename Entity>
  [[nodiscard]] constexpr operator Entity() const noexcept {
    using traits_type = entity_traits<Entity>;
    constexpr auto value = traits_type::construct(traits_type::entity_mask, traits_type::version_mask);
    return value;
  }

  /**
   * @brief Compares two tombstone objects.
   * @param other A tombstone object.
   * @return True in all cases.
   */
  [[nodiscard]] constexpr bool operator==([[maybe_unused]] const entity_tombstone_t other) const noexcept {
    return true;
  }

  /**
   * @brief Compares two tombstone objects.
   * @param other A tombstone object.
   * @return False in all cases.
   */
  [[nodiscard]] constexpr bool operator!=([[maybe_unused]] const entity_tombstone_t other) const noexcept {
    return false;
  }

  /**
   * @brief Compares a tombstone object and an identifier of any type.
   * @tparam Entity Type of identifier.
   * @param entity Identifier with which to compare.
   * @return False if the two elements differ, true otherwise.
   */
  template<typename Entity>
  [[nodiscard]] constexpr bool operator==(const Entity entity) const noexcept {
    using traits_type = entity_traits<Entity>;
    return traits_type::to_version(entity) == traits_type::to_version(*this);
  }

  /**
   * @brief Compares a tombstone object and an identifier of any type.
   * @tparam Entity Type of identifier.
   * @param entity Identifier with which to compare.
   * @return True if the two elements differ, false otherwise.
   */
  template<typename Entity>
  [[nodiscard]] constexpr bool operator!=(const Entity entity) const noexcept {
    return !(entity == *this);
  }
};

/**
 * @brief Compares a tombstone object and an identifier of any type.
 * @tparam Entity Type of identifier.
 * @param entity Identifier with which to compare.
 * @param other A tombstone object yet to be converted.
 * @return False if the two elements differ, true otherwise.
 */
template<typename Entity>
[[nodiscard]] constexpr bool operator==(const Entity entity, const entity_tombstone_t other) noexcept {
  return other.operator==(entity);
}

/**
 * @brief Compares a tombstone object and an identifier of any type.
 * @tparam Entity Type of identifier.
 * @param entity Identifier with which to compare.
 * @param other A tombstone object yet to be converted.
 * @return True if the two elements differ, false otherwise.
 */
template<typename Entity>
[[nodiscard]] constexpr bool operator!=(const Entity entity, const entity_tombstone_t other) noexcept {
  return !(other == entity);
}

/**
 * @brief Compile-time constant for tombstone entities.
 *
 * There exist implicit conversions from this variable to identifiers of any
 * allowed type. Similarly, there exist comparison operators between the
 * tombstone entity and any other identifier.
 */
inline constexpr entity_tombstone_t tombstone{};

} // namespace rendu

#endif //RENDU_CORE_ECS_ENTITY_H_
