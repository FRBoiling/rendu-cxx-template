//
// Created by boil on 2023/3/16.
//

#ifndef TEST_ENUM_FWD_H_
#define TEST_ENUM_FWD_H_

#include <optional>
#include <string_view>
#include <magic_enum/magic_enum.hpp>


enum class Color { RED = 1, GREEN = 2, BLUE = 4 };

enum class Numbers : int {
  one = 1 << 1,
  two = 1 << 2,
  three = 1 << 3,
  many = 1 << 30,
};

enum Directions : std::uint64_t {
  Left = std::uint64_t{1} << 10,
  Down = std::uint64_t{1} << 20,
  Up = std::uint64_t{1} << 31,
  Right = std::uint64_t{1} << 63,
};

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
enum class Language : int {
  日本語 = 1 << 1,
  한국어 = 1 << 2,
  English = 1 << 3,
  😃 = 1 << 4
};
#endif

enum number : unsigned long {
  one = 1 << 1,
  two = 1 << 2,
  three = 1 << 3,
  four = 1 << 4,

#if defined(MAGIC_ENUM_SUPPORTED_ALIASES)
  _1 = one,
  _2 = two,
  _3 = three,
  _4 = four
#endif
};

template<>
struct magic_enum::customize::enum_range<number> {
  static constexpr int min = 100;
  static constexpr int max = 300;
};

template<typename T>
struct MyOpt {
  constexpr MyOpt() : mHasValue{false}, val{} {} // required
  constexpr MyOpt(const T &v) : mHasValue{true}, val{v} {} // required
  constexpr const T &operator*() const { return val; } // required
  constexpr explicit operator bool() const { return mHasValue; } // required

  constexpr bool has_value() const { return mHasValue; }
  constexpr const T &value() const { return val; }

 private:
  bool mHasValue;
  T val;
};

struct MyString {
  MyString() : str{} {} // required
  MyString(const char *s, std::size_t l) : str{s, l} {} // required
  bool empty() const { return str.empty(); } // required
  auto begin() const { return str.begin(); } // required
  auto end() const { return str.end(); } // required
  void append(std::size_t count, char c) { str.append(count, c); } // required
  void append(const char *s, std::size_t size) { str.append(s, size); } // required

  std::size_t size() const { return str.size(); }
  int compare(const char *s) const { return str.compare(s); }

 private:
  std::string str;
};

struct MyStringView {
  using value_type = char; // required
  static constexpr auto npos = std::string_view::npos; // required

  constexpr MyStringView() : str{} {} // required
  constexpr MyStringView(const char *cstr, std::size_t size) : str{cstr, size} {} // required
  constexpr bool empty() const { return str.empty(); } // required
  constexpr std::size_t size() const { return str.size(); } // required
  constexpr const char *data() const { return str.data(); } // required
  constexpr const char &operator[](std::size_t i) const { return str[i]; } // required
  constexpr auto begin() const { return str.begin(); } // required
  constexpr auto end() const { return str.end(); } // required
  constexpr std::size_t find(char c) const { return str.find(c); } // required
  constexpr MyStringView substr(std::size_t p, std::size_t n) { return str.substr(p, n); } // required
  constexpr void remove_prefix(std::size_t n) { str.remove_prefix(n); } // required
  friend constexpr bool operator==(MyStringView lhs, MyStringView rhs); // required

  constexpr MyStringView(const char *cstr) : str{cstr} {}
  constexpr int compare(const char *s) const { return str.compare(s); }

 private:
  std::string_view str;

  constexpr MyStringView(std::string_view s) : str{s} {}
};

constexpr bool operator==(MyStringView lhs, MyStringView rhs) {
  return lhs.str == rhs.str;
}

#endif //TEST_ENUM_FWD_H_
