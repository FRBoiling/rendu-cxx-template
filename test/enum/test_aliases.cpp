/*
* Created by boil on 2022/9/25.
*/

#include "enum_fwd.h"

#include <test/rdtest.h>

#define MAGIC_ENUM_USING_ALIAS_OPTIONAL template<typename T> using optional = MyOpt<T>;
#define MAGIC_ENUM_USING_ALIAS_STRING using string = MyString;
#define MAGIC_ENUM_USING_ALIAS_STRING_VIEW using string_view = MyStringView;

using namespace magic_enum;

TEST(Enum, optional) {
  constexpr auto cr = enum_cast<Color>("RED");
  EXPECT_TRUE(cr.has_value());
  EXPECT_TRUE(cr.value() == Color::RED);

  constexpr auto cn = enum_cast<Color>("NONE");
  EXPECT_FALSE(cn.has_value());
}

TEST(Enum, String) {
  auto cr = enum_flags_name(Color::RED);
  EXPECT_FALSE(cr.empty());
  EXPECT_TRUE(cr.compare("RED") == 0);
  using namespace bitwise_operators;
  auto crg = enum_flags_name(Color::RED | Color::GREEN);
  EXPECT_FALSE(crg.empty());
  EXPECT_TRUE(crg.compare("RED|GREEN") == 0);

  auto cn = enum_flags_name(Color{0});
  EXPECT_TRUE(cn.empty());
  EXPECT_TRUE(cn.size() == 0);
}

TEST(Enum, StringView) {
  auto cr = enum_name(Color::RED);
  EXPECT_FALSE(cr.empty());
  EXPECT_TRUE(cr.compare("RED") == 0);

  auto cn = enum_name(Color{0});
  EXPECT_TRUE(cn.empty());
  EXPECT_TRUE(cn.size() == 0);
}
