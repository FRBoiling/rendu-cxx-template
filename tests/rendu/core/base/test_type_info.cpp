/*
* Created by boil on 2023/2/19.
*/

#include <test/rdtest.h>
#include <core/base/type_info.h>
#include <core/base/type_traits.h>

template<>
struct rendu::type_name<float> final {
  [[nodiscard]] static constexpr std::string_view value() noexcept {
    return std::string_view{""};
  }
};

TEST(TypeIndex, Functionalities) {
  ASSERT_EQ(rendu::type_index<int>::value(), rendu::type_index<int>::value());
  ASSERT_NE(rendu::type_index<int>::value(), rendu::type_index<char>::value());
  ASSERT_NE(rendu::type_index<int>::value(), rendu::type_index<int &&>::value());
  ASSERT_NE(rendu::type_index<int &>::value(), rendu::type_index<const int &>::value());
  ASSERT_EQ(static_cast<rendu::id_type>(rendu::type_index<int>{}), rendu::type_index<int>::value());
}

TEST(TypeHash, Functionalities) {
  ASSERT_NE(rendu::type_hash<int>::value(), rendu::type_hash<const int>::value());
  ASSERT_NE(rendu::type_hash<int>::value(), rendu::type_hash<char>::value());
  ASSERT_EQ(rendu::type_hash<int>::value(), rendu::type_hash<int>::value());
  ASSERT_EQ(static_cast<rendu::id_type>(rendu::type_hash<int>{}), rendu::type_hash<int>::value());
}

TEST(TypeName, Functionalities) {
  ASSERT_EQ(rendu::type_name<int>::value(), std::string_view{"int"});
  ASSERT_EQ(rendu::type_name<float>{}.value(), std::string_view{""});

  ASSERT_TRUE((rendu::type_name<rendu::integral_constant<3>>::value() == std::string_view{"std::integral_constant<int, 3>"})
                  || (rendu::type_name<rendu::integral_constant<3>>::value() == std::string_view{"std::__1::integral_constant<int, 3>"})
                  || (rendu::type_name<rendu::integral_constant<3>>::value() == std::string_view{"struct std::integral_constant<int,3>"}));

  ASSERT_TRUE(((rendu::type_name<rendu::type_list<rendu::type_list<int, char>, double>>::value()) == std::string_view{"rendu::type_list<rendu::type_list<int, char>, double>"})
                  || ((rendu::type_name<rendu::type_list<rendu::type_list<int, char>, double>>::value()) == std::string_view{"struct rendu::type_list<struct rendu::type_list<int,char>,double>"}));

  ASSERT_EQ(static_cast<std::string_view>(rendu::type_name<int>{}), rendu::type_name<int>::value());
}

TEST(TypeInfo, Functionalities) {
  static_assert(std::is_copy_constructible_v<rendu::type_info>);
  static_assert(std::is_move_constructible_v<rendu::type_info>);
  static_assert(std::is_copy_assignable_v<rendu::type_info>);
  static_assert(std::is_move_assignable_v<rendu::type_info>);

  rendu::type_info info{std::in_place_type<int>};
  rendu::type_info other{std::in_place_type<void>};

  ASSERT_EQ(info, rendu::type_info{std::in_place_type<int &>});
  ASSERT_EQ(info, rendu::type_info{std::in_place_type<int &&>});
  ASSERT_EQ(info, rendu::type_info{std::in_place_type<const int &>});

  ASSERT_NE(info, other);
  ASSERT_TRUE(info == info);
  ASSERT_FALSE(info != info);

  ASSERT_EQ(info.index(), rendu::type_index<int>::value());
  ASSERT_EQ(info.hash(), rendu::type_hash<int>::value());
  ASSERT_EQ(info.name(), rendu::type_name<int>::value());

  other = info;

  ASSERT_EQ(other.index(), rendu::type_index<int>::value());
  ASSERT_EQ(other.hash(), rendu::type_hash<int>::value());
  ASSERT_EQ(other.name(), rendu::type_name<int>::value());

  ASSERT_EQ(other.index(), info.index());
  ASSERT_EQ(other.hash(), info.hash());
  ASSERT_EQ(other.name(), info.name());

  other = std::move(info);

  ASSERT_EQ(other.index(), rendu::type_index<int>::value());
  ASSERT_EQ(other.hash(), rendu::type_hash<int>::value());
  ASSERT_EQ(other.name(), rendu::type_name<int>::value());

  ASSERT_EQ(other.index(), info.index());
  ASSERT_EQ(other.hash(), info.hash());
  ASSERT_EQ(other.name(), info.name());
}

TEST(TypeInfo, Order) {
  rendu::type_info rhs = rendu::type_id<int>();
  rendu::type_info lhs = rendu::type_id<char>();

  // let's adjust the two objects since values are generated at runtime
  rhs < lhs ? void() : std::swap(lhs, rhs);

  ASSERT_FALSE(lhs < lhs);
  ASSERT_FALSE(rhs < rhs);

  ASSERT_LT(rhs, lhs);
  ASSERT_LE(rhs, lhs);

  ASSERT_GT(lhs, rhs);
  ASSERT_GE(lhs, rhs);
}

TEST(TypeId, Functionalities) {
  const int value = 42;

  ASSERT_EQ(rendu::type_id(value), rendu::type_id<int>());
  ASSERT_EQ(rendu::type_id(42), rendu::type_id<int>());

  ASSERT_EQ(rendu::type_id<int>(), rendu::type_id<int>());
  ASSERT_EQ(rendu::type_id<int &>(), rendu::type_id<int &&>());
  ASSERT_EQ(rendu::type_id<int &>(), rendu::type_id<int>());
  ASSERT_NE(rendu::type_id<int>(), rendu::type_id<char>());

  ASSERT_EQ(&rendu::type_id<int>(), &rendu::type_id<int>());
  ASSERT_NE(&rendu::type_id<int>(), &rendu::type_id<void>());
}
