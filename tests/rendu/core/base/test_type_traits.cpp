/*
* Created by boil on 2023/2/19.
*/
#include <unordered_map>
#include <test/rdtest.h>
#include <core/base/hashed_string.h>
#include <core/base/type_traits.h>

struct not_comparable {
  bool operator==(const not_comparable &) const = delete;
};

struct nlohmann_json_like final {
  using value_type = nlohmann_json_like;

  bool operator==(const nlohmann_json_like &) const {
    return true;
  }
};

struct clazz {
  char foo(int) {
    return {};
  }

  int bar(double, float) const {
    return {};
  }

  bool quux;
};

int free_function(int, const double &) {
  return 42;
}

template<typename, typename Type = void>
struct multi_argument_operation {
  using type = Type;
};

TEST(SizeOf, Functionalities) {
  static_assert(rendu::size_of_v<void> == 0u);
  static_assert(rendu::size_of_v<char> == sizeof(char));
  static_assert(rendu::size_of_v<int[]> == 0u);
  static_assert(rendu::size_of_v<int[3]> == sizeof(int[3]));
}

TEST(UnpackAsType, Functionalities) {
  auto test = [](auto &&...args) {
    return [](rendu::unpack_as_type<int, decltype(args)>... value) {
      return (value + ... + 0);
    };
  };

  ASSERT_EQ(test('c', 42., true)(1, 2, 3), 6);
}

TEST(UnpackAsValue, Functionalities) {
  auto test = [](auto &&...args) {
    return (rendu::unpack_as_value<2, decltype(args)> + ... + 0);
  };

  ASSERT_EQ(test('c', 42., true), 6);
}

TEST(IntegralConstant, Functionalities) {
  rendu::integral_constant<3> constant{};

  static_assert(std::is_same_v<typename rendu::integral_constant<3>::value_type, int>);
  static_assert(constant.value == 3);
}

TEST(Choice, Functionalities) {
  static_assert(std::is_base_of_v<rendu::choice_t<0>, rendu::choice_t<1>>);
  static_assert(!std::is_base_of_v<rendu::choice_t<1>, rendu::choice_t<0>>);
}

TEST(TypeList, Functionalities) {
  using type = rendu::type_list<int, char>;
  using other = rendu::type_list<double>;

  static_assert(type::size == 2u);
  static_assert(other::size == 1u);

  static_assert(std::is_same_v<decltype(type{} + other{}), rendu::type_list<int, char, double>>);
  static_assert(std::is_same_v<rendu::type_list_cat_t<type, other, type, other>,
                               rendu::type_list<int, char, double, int, char, double>>);
  static_assert(std::is_same_v<rendu::type_list_cat_t<type, other>, rendu::type_list<int, char, double>>);
  static_assert(std::is_same_v<rendu::type_list_cat_t<type, type>, rendu::type_list<int, char, int, char>>);
  static_assert(std::is_same_v<rendu::type_list_unique_t<rendu::type_list_cat_t<type, type>>,
                               rendu::type_list<int, char>>);

  static_assert(rendu::type_list_contains_v<type, int>);
  static_assert(rendu::type_list_contains_v<type, char>);
  static_assert(!rendu::type_list_contains_v<type, double>);

  static_assert(std::is_same_v<rendu::type_list_element_t<0u, type>, int>);
  static_assert(std::is_same_v<rendu::type_list_element_t<1u, type>, char>);
  static_assert(std::is_same_v<rendu::type_list_element_t<0u, other>, double>);

  static_assert(rendu::type_list_index_v<int, type> == 0u);
  static_assert(rendu::type_list_index_v<char, type> == 1u);
  static_assert(rendu::type_list_index_v<double, other> == 0u);

  static_assert(std::is_same_v<rendu::type_list_diff_t<rendu::type_list<int, char, double>,
                                                       rendu::type_list<float, bool>>,
                               rendu::type_list<int, char, double>>);
  static_assert(std::is_same_v<rendu::type_list_diff_t<rendu::type_list<int, char, double>,
                                                       rendu::type_list<int, char, double>>, rendu::type_list<>>);
  static_assert(std::is_same_v<rendu::type_list_diff_t<rendu::type_list<int, char, double>,
                                                       rendu::type_list<int, char>>, rendu::type_list<double>>);
  static_assert(std::is_same_v<rendu::type_list_diff_t<rendu::type_list<int, char, double>,
                                                       rendu::type_list<char, double>>, rendu::type_list<int>>);
  static_assert(std::is_same_v<rendu::type_list_diff_t<rendu::type_list<int, char, double>, rendu::type_list<char>>,
                               rendu::type_list<int, double>>);

  static_assert(std::is_same_v<rendu::type_list_transform_t<rendu::type_list<int, char>, rendu::type_identity>,
                               rendu::type_list<int, char>>);
  static_assert(std::is_same_v<rendu::type_list_transform_t<rendu::type_list<int, char>, std::add_const>,
                               rendu::type_list<const int, const char>>);
  static_assert(std::is_same_v<rendu::type_list_transform_t<rendu::type_list<int, char>, multi_argument_operation>,
                               rendu::type_list<void, void>>);
}

TEST(ValueList, Functionalities) {
  using value = rendu::value_list<0, 2>;
  using other = rendu::value_list<1>;

  static_assert(value::size == 2u);
  static_assert(other::size == 1u);

  static_assert(std::is_same_v<decltype(value{} + other{}), rendu::value_list<0, 2, 1>>);
  static_assert(std::is_same_v<rendu::value_list_cat_t<value, other, value, other>,
                               rendu::value_list<0, 2, 1, 0, 2, 1>>);
  static_assert(std::is_same_v<rendu::value_list_cat_t<value, other>, rendu::value_list<0, 2, 1>>);
  static_assert(std::is_same_v<rendu::value_list_cat_t<value, value>, rendu::value_list<0, 2, 0, 2>>);

  static_assert(rendu::value_list_element_v<0u, value> == 0);
  static_assert(rendu::value_list_element_v<1u, value> == 2);
  static_assert(rendu::value_list_element_v<0u, other> == 1);
}

TEST(IsApplicable, Functionalities) {
  static_assert(rendu::is_applicable_v<void(int, char), std::tuple<double, char>>);
  static_assert(!rendu::is_applicable_v<void(int, char), std::tuple<int>>);

  static_assert(rendu::is_applicable_r_v<float, int(int, char), std::tuple<double, char>>);
  static_assert(!rendu::is_applicable_r_v<float, void(int, char), std::tuple<double, char>>);
  static_assert(!rendu::is_applicable_r_v<int, int(int, char), std::tuple<void>>);
}

TEST(IsComplete, Functionalities) {
  static_assert(!rendu::is_complete_v<void>);
  static_assert(rendu::is_complete_v<int>);
}

TEST(IsIterator, Functionalities) {
  static_assert(!rendu::is_iterator_v<void>);
  static_assert(!rendu::is_iterator_v<int>);

  static_assert(!rendu::is_iterator_v<void *>);
  static_assert(rendu::is_iterator_v<int *>);

  static_assert(rendu::is_iterator_v<std::vector<int>::iterator>);
  static_assert(rendu::is_iterator_v<std::vector<int>::const_iterator>);
  static_assert(rendu::is_iterator_v<std::vector<int>::reverse_iterator>);
}

TEST(IsEBCOEligible, Functionalities) {
  static_assert(rendu::is_ebco_eligible_v<not_comparable>);
  static_assert(!rendu::is_ebco_eligible_v<nlohmann_json_like>);
  static_assert(!rendu::is_ebco_eligible_v<double>);
  static_assert(!rendu::is_ebco_eligible_v<void>);
}

TEST(IsTransparent, Functionalities) {
  static_assert(!rendu::is_transparent_v<std::less<int>>);
  static_assert(rendu::is_transparent_v<std::less<void>>);
  static_assert(!rendu::is_transparent_v<std::logical_not<double>>);
  static_assert(rendu::is_transparent_v<std::logical_not<void>>);
}

TEST(IsEqualityComparable, Functionalities) {
  static_assert(rendu::is_equality_comparable_v<int>);
  static_assert(rendu::is_equality_comparable_v<const int>);
  static_assert(rendu::is_equality_comparable_v<std::vector<int>>);
  static_assert(rendu::is_equality_comparable_v<std::vector<std::vector<int>>>);
  static_assert(rendu::is_equality_comparable_v<std::unordered_map<int, int>>);
  static_assert(rendu::is_equality_comparable_v<std::unordered_map<int, std::unordered_map<int, char>>>);
  static_assert(rendu::is_equality_comparable_v<std::pair<const int, int>>);
  static_assert(rendu::is_equality_comparable_v<std::pair<const int, std::unordered_map<int, char>>>);
  static_assert(rendu::is_equality_comparable_v<std::vector<not_comparable>::iterator>);
  static_assert(rendu::is_equality_comparable_v<nlohmann_json_like>);

  static_assert(rendu::is_equality_comparable_v<int[3u]>);
  static_assert(!rendu::is_equality_comparable_v<not_comparable>);
  static_assert(!rendu::is_equality_comparable_v<const not_comparable>);
  static_assert(!rendu::is_equality_comparable_v<std::vector<not_comparable>>);
  static_assert(!rendu::is_equality_comparable_v<std::vector<std::vector<not_comparable>>>);
  static_assert(!rendu::is_equality_comparable_v<std::unordered_map<int, not_comparable>>);
  static_assert(!rendu::is_equality_comparable_v<std::unordered_map<int, std::unordered_map<int, not_comparable>>>);
  static_assert(!rendu::is_equality_comparable_v<std::pair<const int, not_comparable>>);
  static_assert(!rendu::is_equality_comparable_v<std::pair<const int, std::unordered_map<int, not_comparable>>>);
  static_assert(!rendu::is_equality_comparable_v<void>);
}

TEST(ConstnessAs, Functionalities) {
  static_assert(std::is_same_v<rendu::constness_as_t<int, char>, int>);
  static_assert(std::is_same_v<rendu::constness_as_t<const int, char>, int>);
  static_assert(std::is_same_v<rendu::constness_as_t<int, const char>, const int>);
  static_assert(std::is_same_v<rendu::constness_as_t<const int, const char>, const int>);
}

TEST(MemberClass, Functionalities) {
  static_assert(std::is_same_v<clazz, rendu::member_class_t<decltype(&clazz::foo)>>);
  static_assert(std::is_same_v<clazz, rendu::member_class_t<decltype(&clazz::bar)>>);
  static_assert(std::is_same_v<clazz, rendu::member_class_t<decltype(&clazz::quux)>>);
}

TEST(NthArgument, Functionalities) {
  static_assert(std::is_same_v<rendu::nth_argument_t<0u, &free_function>, int>);
  static_assert(std::is_same_v<rendu::nth_argument_t<1u, &free_function>, const double &>);
  static_assert(std::is_same_v<rendu::nth_argument_t<0u, &clazz::bar>, double>);
  static_assert(std::is_same_v<rendu::nth_argument_t<1u, &clazz::bar>, float>);
  static_assert(std::is_same_v<rendu::nth_argument_t<0u, &clazz::quux>, bool>);

  ASSERT_EQ(free_function(rendu::nth_argument_t<0u, &free_function>{}, rendu::nth_argument_t<1u, &free_function>{}),
            42);
}

TEST(Tag, Functionalities) {
  using namespace rendu::literals;
  static_assert(rendu::tag<"foobar"_hs>::value == rendu::hashed_string::value("foobar"));
  static_assert(std::is_same_v<typename rendu::tag<"foobar"_hs>::value_type, rendu::id_type>);
}
