/*
* Created by boil on 2022/9/25.
*/

#include <optional>
#include <string_view>

#include <test/rdtest.h>
#include <enum.h>

using namespace rendu;
using namespace rendu::bitwise_operators;
namespace {

template <typename T>
struct MyOpt {
  constexpr MyOpt() : mHasValue{false}, val{} {} // required
  constexpr MyOpt(const T& v) : mHasValue{true}, val{v} {} // required
  constexpr const T& operator*() const { return val; } // required
  constexpr explicit operator bool() const  { return mHasValue; } // required

  constexpr bool has_value() const { return mHasValue; }
  constexpr const T& value() const { return val; }

 private:
  bool mHasValue;
  T val;
};

struct MyString {
  MyString() : str{} {} // required
  MyString(const char* s, std::size_t l) : str{s, l} {} // required
  bool empty() const { return str.empty(); } // required
  auto begin() const { return str.begin(); } // required
  auto end() const { return str.end(); } // required
  void append(std::size_t count, char c) { str.append(count, c); } // required
  void append(const char* s, std::size_t size) { str.append(s, size); } // required

  std::size_t size() const { return str.size(); }
  int compare(const char* s) const { return str.compare(s); }

 private:
  std::string str;
};

struct MyStringView {
  using value_type = char; // required
  static constexpr auto npos = std::string_view::npos; // required

  constexpr MyStringView() : str{} {} // required
  constexpr MyStringView(const char* cstr, std::size_t size) : str{cstr, size} {} // required
  constexpr bool empty() const { return str.empty(); } // required
  constexpr std::size_t size() const { return str.size(); } // required
  constexpr const char* data() const { return str.data(); } // required
  constexpr const char& operator[](std::size_t i) const { return str[i]; } // required
  constexpr auto begin() const { return str.begin(); } // required
  constexpr auto end() const { return str.end(); } // required
  constexpr std::size_t find(char c) const { return str.find(c); } // required
  constexpr MyStringView substr(std::size_t p, std::size_t n) { return str.substr(p, n); } // required
  constexpr void remove_prefix(std::size_t n) { str.remove_prefix(n); } // required
  friend constexpr bool operator==(MyStringView lhs, MyStringView rhs); // required

  constexpr MyStringView(const char* cstr) : str{ cstr } {}
  constexpr int compare(const char* s) const { return str.compare(s); }

 private:
  std::string_view str;

  constexpr MyStringView(std::string_view s) : str{s} {}
};

constexpr bool operator==(MyStringView lhs, MyStringView rhs) {
  return lhs.str == rhs.str;
}

#define MAGIC_ENUM_USING_ALIAS_OPTIONAL template<typename T> using optional = MyOpt<T>;
#define MAGIC_ENUM_USING_ALIAS_STRING using string = MyString;
#define MAGIC_ENUM_USING_ALIAS_STRING_VIEW using string_view = MyStringView;

enum class Color { RED = 1, GREEN = 2, BLUE = 4 };

TEST(Enum,optional) {
  constexpr auto cr = enum_cast<Color>("RED");
  EXPECT_TRUE(cr.has_value());
  EXPECT_TRUE(cr.value() == Color::RED);

  constexpr auto cn = enum_cast<Color>("NONE");
  RD_EXPECT_FALSE(cn.has_value());
}

TEST(Enum,String) {
  auto cr = enum_flags_name(Color::RED);
  RD_EXPECT_FALSE(cr.empty());
  EXPECT_TRUE(cr.compare("RED") == 0);

  auto crg = enum_flags_name(Color::RED | Color::GREEN);
  RD_EXPECT_FALSE(crg.empty());
  EXPECT_TRUE(crg.compare("RED|GREEN") == 0);

  auto cn = enum_flags_name(Color{0});
  EXPECT_TRUE(cn.empty());
  EXPECT_TRUE(cn.size() == 0);
}

TEST(Enum,StringView) {
  auto cr = enum_name(Color::RED);
  RD_EXPECT_FALSE(cr.empty());
  EXPECT_TRUE(cr.compare("RED") == 0);

  auto cn = enum_name(Color{0});
  EXPECT_TRUE(cn.empty());
  EXPECT_TRUE(cn.size() == 0);
}
}
