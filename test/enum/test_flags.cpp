/*
* Created by boil on 2022/9/25.
*/
#include <test/rdtest.h>
#include "enum.h"
namespace {

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
struct rendu::customize::enum_range<number> {
  static constexpr int min = 100;
  static constexpr int max = 300;
};

using namespace rendu;
using namespace rendu::bitwise_operators;

RD_TEST(Flags, enum_string) {
  constexpr auto cr = enum_cast<Color>("RED");
  RD_EXPECT_TRUE(cr.value() == Color::RED);
  RD_EXPECT_TRUE(enum_cast<Color &>("GREEN").value() == Color::GREEN);
  RD_EXPECT_TRUE(
      enum_cast<Color>("blue", [](char lhs, char rhs) { return std::tolower(lhs) == std::tolower(rhs); }).value()
          == Color::BLUE);
  RD_EXPECT_TRUE(enum_cast<Color &>("blue|RED",
                                    [](char lhs, char rhs) { return std::tolower(lhs) == std::tolower(rhs); }).value()
                     == (Color::BLUE | Color::RED));
  RD_EXPECT_TRUE(enum_cast<Color &>("GREEN|RED").value() == (Color::GREEN | Color::RED));
  RD_EXPECT_TRUE(enum_cast<Color &>("GREEN|RED|RED").value() == (Color::GREEN | Color::RED));
  RD_EXPECT_FALSE(enum_cast<Color &>("GREEN|RED|None").has_value());
  RD_EXPECT_FALSE(enum_cast<Color>("None").has_value());

  constexpr auto no = enum_cast<Numbers>("one");
  RD_EXPECT_TRUE(no.value() == Numbers::one);
  RD_EXPECT_TRUE(enum_cast<Numbers>("two").value() == Numbers::two);
  RD_EXPECT_TRUE(enum_cast<Numbers>("three").value() == Numbers::three);
  RD_EXPECT_TRUE(enum_cast<Numbers>("many") == Numbers::many);
  RD_EXPECT_FALSE(enum_cast<Numbers>("None").has_value());

  constexpr auto dr = enum_cast<Directions>("Right");
  RD_EXPECT_TRUE(enum_cast<Directions &>("Up").value() == Directions::Up);
  RD_EXPECT_TRUE(enum_cast<const Directions>("Down").value() == Directions::Down);
  RD_EXPECT_TRUE(dr.value() == Directions::Right);
  RD_EXPECT_TRUE(enum_cast<Directions>("Left").value() == Directions::Left);
  RD_EXPECT_FALSE(enum_cast<Directions>("None").has_value());

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto lang = enum_cast<Language>("日本語");
  RD_EXPECT_TRUE(enum_cast<Language&>("한국어").value() == Language::한국어);
  RD_EXPECT_TRUE(enum_cast<const Language>("English").value() == Language::English);
  RD_EXPECT_TRUE(lang.value() == Language::日本語);
  RD_EXPECT_TRUE(enum_cast<Language>("😃").value() == Language::😃);
  RD_EXPECT_FALSE(enum_cast<Language>("None").has_value());
#endif

  constexpr auto nto = enum_cast<number>("three|one");
  RD_EXPECT_TRUE(enum_cast<number>("one").value() == number::one);
  RD_EXPECT_TRUE(enum_cast<number>("two").value() == number::two);
  RD_EXPECT_TRUE(enum_cast<number>("three").value() == number::three);
  RD_EXPECT_TRUE(enum_cast<number>("four") == number::four);
  RD_EXPECT_TRUE(nto.value() == (number::three | number::one));
  RD_EXPECT_FALSE(enum_cast<number>("None").has_value());
}

RD_TEST(Flags, enum_integer) {
  Color cm[3] = {Color::RED, Color::GREEN, Color::BLUE};
  constexpr auto cr = enum_cast<Color>(1);
  RD_EXPECT_TRUE(cr.value() == Color::RED);
  RD_EXPECT_TRUE(enum_cast<Color &>(2).value() == Color::GREEN);
  RD_EXPECT_TRUE(enum_cast<Color>(static_cast<int>(cm[2])).value() == Color::BLUE);
  RD_EXPECT_TRUE(enum_cast<Color>(1 | 2).value() == (Color::GREEN | Color::RED));
  RD_EXPECT_TRUE(enum_cast<Color>(1 | 2 | 1).value() == (Color::GREEN | Color::RED));
  RD_EXPECT_FALSE(enum_cast<Color>(1 | 2 | 8).has_value());
  RD_EXPECT_FALSE(enum_cast<Color>(0).has_value());

  constexpr auto no = enum_cast<Numbers>(2);
  RD_EXPECT_TRUE(no.value() == Numbers::one);
  RD_EXPECT_TRUE(enum_cast<Numbers>(4).value() == Numbers::two);
  RD_EXPECT_TRUE(enum_cast<Numbers>(8).value() == Numbers::three);
  RD_EXPECT_TRUE(enum_cast<Numbers>(1 << 30).value() == Numbers::many);
  RD_EXPECT_FALSE(enum_cast<Numbers>(127).has_value());
  RD_EXPECT_FALSE(enum_cast<Numbers>(0).has_value());

  constexpr auto dr = enum_cast<Directions>(std::uint64_t{1} << 63);
  RD_EXPECT_TRUE(enum_cast<Directions &>(std::uint64_t{1} << 31).value() == Directions::Up);
  RD_EXPECT_TRUE(enum_cast<const Directions>(std::uint64_t{1} << 20).value() == Directions::Down);
  RD_EXPECT_TRUE(dr.value() == Directions::Right);
  RD_EXPECT_TRUE(enum_cast<Directions>(std::uint64_t{1} << 10).value() == Directions::Left);
  RD_EXPECT_FALSE(enum_cast<Directions>(0).has_value());

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto lang = enum_cast<Language>(1 << 1);
  RD_EXPECT_TRUE(enum_cast<Language&>(1 << 2).value() == Language::한국어);
  RD_EXPECT_TRUE(enum_cast<const Language>(1 << 3).value() == Language::English);
  RD_EXPECT_TRUE(lang.value() == Language::日本語);
  RD_EXPECT_TRUE(enum_cast<Language>(1 << 4).value() == Language::😃);
  RD_EXPECT_FALSE(enum_cast<Language>(0).has_value());
#endif

  constexpr auto nto = enum_cast<number>(2 | 8);
  RD_EXPECT_TRUE(enum_cast<number>(1 << 1).value() == number::one);
  RD_EXPECT_TRUE(enum_cast<number>(1 << 2).value() == number::two);
  RD_EXPECT_TRUE(enum_cast<number>(1 << 3).value() == number::three);
  RD_EXPECT_TRUE(enum_cast<number>(1 << 4).value() == number::four);
  RD_EXPECT_TRUE(nto.value() == (number::three | number::one));
  RD_EXPECT_FALSE(enum_cast<number>(0).has_value());
}

RD_TEST(Flags, enum_index2) {
  Color cm[3] = {Color::RED, Color::GREEN, Color::BLUE};
  constexpr auto cr = enum_index(Color::RED);
  Color cg = Color::GREEN;
  RD_EXPECT_TRUE(cr.value() == 0);
  RD_EXPECT_TRUE(enum_index<Color &>(cg).value() == 1);
  RD_EXPECT_TRUE(enum_index(cm[2]).value() == 2);
  RD_EXPECT_FALSE(enum_index<Color>(Color::RED | Color::GREEN).has_value());
  RD_EXPECT_FALSE(enum_index<Color>(Color::RED | Color::GREEN | Color::RED).has_value());
  RD_EXPECT_FALSE(enum_index<Color>(Color::RED | Color{8}).has_value());
  RD_EXPECT_FALSE(enum_index(static_cast<Color>(0)).has_value());

  constexpr auto no = enum_index(Numbers::one);
  RD_EXPECT_TRUE(no.value() == 0);
  RD_EXPECT_TRUE(enum_index(Numbers::two).value() == 1);
  RD_EXPECT_TRUE(enum_index(Numbers::three).value() == 2);
  RD_EXPECT_TRUE(enum_index(Numbers::many).value() == 3);
  RD_EXPECT_FALSE(enum_index(static_cast<Numbers>(0)).has_value());

  constexpr auto dr = enum_index(Directions::Right);
  Directions dl = Directions::Left;
  RD_EXPECT_TRUE(enum_index<Directions &>(dl).value() == 0);
  RD_EXPECT_TRUE(enum_index<const Directions>(Directions::Down).value() == 1);
  RD_EXPECT_TRUE(enum_index(Directions::Up).value() == 2);
  RD_EXPECT_TRUE(dr.value() == 3);
  RD_EXPECT_FALSE(enum_index(static_cast<Directions>(0)).has_value());

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto lang = enum_index<Language>(Language::日本語);
  Language korean = Language::한국어;
  RD_EXPECT_TRUE(enum_index<Language&>(korean).value() == 1);
  RD_EXPECT_TRUE(enum_index<const Language>(Language::English).value() == 2);
  RD_EXPECT_TRUE(enum_index(Language::😃).value() == 3);
  RD_EXPECT_TRUE(lang.value() == 0);
  RD_EXPECT_FALSE(enum_index(static_cast<Language>(0)).has_value());
#endif

  constexpr auto nto = enum_index(number::three | number::one);
  RD_EXPECT_TRUE(enum_index(number::one).value() == 0);
  RD_EXPECT_TRUE(enum_index(number::two).value() == 1);
  RD_EXPECT_TRUE(enum_index(number::three).value() == 2);
  RD_EXPECT_TRUE(enum_index(number::four).value() == 3);
  RD_EXPECT_FALSE(nto.has_value());
  RD_EXPECT_FALSE(enum_index(static_cast<number>(0)).has_value());
}

RD_TEST(Flags, enum_contains_value) {
  Color cm[3] = {Color::RED, Color::GREEN, Color::BLUE};
  constexpr auto cr = enum_contains(Color::RED);
  Color cg = Color::GREEN;
  RD_EXPECT_TRUE(cr);
  RD_EXPECT_TRUE(enum_contains<Color &>(cg));
  RD_EXPECT_TRUE(enum_contains(cm[2]));
  RD_EXPECT_TRUE(enum_contains<Color>(Color::RED | Color::GREEN));
  RD_EXPECT_TRUE(enum_contains<Color>(Color::RED | Color::GREEN | Color::GREEN));
  RD_EXPECT_FALSE(enum_contains<Color>(Color::RED | Color{8}));
  RD_EXPECT_FALSE(enum_contains(static_cast<Color>(0)));

  constexpr auto no = enum_contains(Numbers::one);
  RD_EXPECT_TRUE(no);
  RD_EXPECT_TRUE(enum_contains(Numbers::two));
  RD_EXPECT_TRUE(enum_contains(Numbers::three));
  RD_EXPECT_TRUE(enum_contains(Numbers::many));
  RD_EXPECT_FALSE(enum_contains(static_cast<Numbers>(0)));

  constexpr auto dr = enum_contains(Directions::Right);
  Directions dl = Directions::Left;
  RD_EXPECT_TRUE(enum_contains<Directions &>(dl));
  RD_EXPECT_TRUE(enum_contains<const Directions>(Directions::Down));
  RD_EXPECT_TRUE(enum_contains(Directions::Up));
  RD_EXPECT_TRUE(dr);
  RD_EXPECT_FALSE(enum_contains(static_cast<Directions>(0)));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto lang = enum_index<Language>(Language::日本語);
  Language korean = Language::한국어;
  RD_EXPECT_TRUE(enum_contains<Language&>(korean));
  RD_EXPECT_TRUE(enum_contains<const Language>(Language::English));
  RD_EXPECT_TRUE(enum_contains(Language::😃));
  RD_EXPECT_TRUE(lang);
  RD_EXPECT_FALSE(enum_contains(static_cast<Language>(0)));
#endif

  constexpr auto nto = enum_contains(number::three | number::one);
  RD_EXPECT_TRUE(enum_contains(number::one));
  RD_EXPECT_TRUE(enum_contains<number &>(number::two));
  RD_EXPECT_TRUE(enum_contains(number::one));
  RD_EXPECT_TRUE(enum_contains(number::four));
  RD_EXPECT_TRUE(nto);
  RD_EXPECT_FALSE(enum_contains(static_cast<number>(0)));
}

RD_TEST(Flags, enum_contains_integer) {
  RD_EXPECT_TRUE(enum_contains<Color>(1));
  RD_EXPECT_TRUE(enum_contains<Color &>(2));
  RD_EXPECT_TRUE(enum_contains<Color>(4));
  RD_EXPECT_TRUE(enum_contains<Color>(1 | 2));
  RD_EXPECT_TRUE(enum_contains<Color>(1 | 2 | 1));
  RD_EXPECT_FALSE(enum_contains<Color>(1 | 2 | 8));
  RD_EXPECT_FALSE(enum_contains<Color>(0));

  constexpr auto no = enum_contains<Numbers>(1 << 1);
  RD_EXPECT_TRUE(no);
  RD_EXPECT_TRUE(enum_contains<Numbers>(1 << 2));
  RD_EXPECT_TRUE(enum_contains<Numbers>(1 << 3));
  RD_EXPECT_TRUE(enum_contains<Numbers>(1 << 30));

  constexpr auto dr = enum_contains<Directions &>(std::uint64_t{1} << 63);
  RD_EXPECT_TRUE(dr);
  RD_EXPECT_TRUE(enum_contains<const Directions>(std::uint64_t{1} << 10));
  RD_EXPECT_TRUE(enum_contains<Directions>(std::uint64_t{1} << 20));
  RD_EXPECT_TRUE(enum_contains<Directions>(std::uint64_t{1} << 31));
  RD_EXPECT_FALSE(enum_contains<Directions>(static_cast<Directions>(0)));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto lang = enum_contains<Language&>(1 << 1);
  RD_EXPECT_TRUE(lang);
  RD_EXPECT_TRUE(enum_contains<const Language>(1 << 2));
  RD_EXPECT_TRUE(enum_contains<Language>(1 << 3));
  RD_EXPECT_TRUE(enum_contains<Language>(1 << 4));
  RD_EXPECT_FALSE(enum_contains(static_cast<Language>(0)));
#endif

  constexpr auto nto = enum_contains<number>(8 | 2);
  RD_EXPECT_TRUE(enum_contains<number>(1 << 1));
  RD_EXPECT_TRUE(enum_contains<number>(1 << 2));
  RD_EXPECT_TRUE(enum_contains<number>(1 << 3));
  RD_EXPECT_TRUE(enum_contains<number>(1 << 4));
  RD_EXPECT_TRUE(enum_contains<number>(8 | 2 | 16));
  RD_EXPECT_TRUE(enum_contains<number>(8 | 16 | 16));
  RD_EXPECT_TRUE(nto);
  RD_EXPECT_FALSE(enum_contains<number>(8 | 64));
  RD_EXPECT_FALSE(enum_contains<number>(0));
}

RD_TEST(Flags, enum_contains_string) {
  constexpr auto cr = "RED";
  RD_EXPECT_TRUE(enum_contains<Color>(cr));
  RD_EXPECT_TRUE(enum_contains<Color &>("GREEN"));
  RD_EXPECT_TRUE(enum_contains<Color>("blue",
                                      [](char lhs, char rhs) { return std::tolower(lhs) == std::tolower(rhs); }));
  RD_EXPECT_TRUE(enum_contains<Color &>("blue|RED",
                                        [](char lhs, char rhs) { return std::tolower(lhs) == std::tolower(rhs); }));
  RD_EXPECT_TRUE(enum_contains<Color &>("GREEN|RED"));
  RD_EXPECT_TRUE(enum_contains<Color &>("GREEN|RED|RED"));
  RD_EXPECT_FALSE(enum_contains<Color>("GREEN|RED|None"));
  RD_EXPECT_FALSE(enum_contains<Color>("None"));

  constexpr auto no = std::string_view{"one"};
  RD_EXPECT_TRUE(enum_contains<Numbers>(no));
  RD_EXPECT_TRUE(enum_contains<Numbers>("two"));
  RD_EXPECT_TRUE(enum_contains<Numbers>("three"));
  RD_EXPECT_TRUE(enum_contains<Numbers>("many"));
  RD_EXPECT_FALSE(enum_contains<Numbers>("None"));

  auto dr = std::string{"Right"};
  RD_EXPECT_TRUE(enum_contains<Directions &>("Up"));
  RD_EXPECT_TRUE(enum_contains<Directions>("Down"));
  RD_EXPECT_TRUE(enum_contains<const Directions>(dr));
  RD_EXPECT_TRUE(enum_contains<Directions>("Left"));
  RD_EXPECT_FALSE(enum_contains<Directions>("None"));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  auto lang = std::string{"日本語"};
  RD_EXPECT_TRUE(enum_contains<Language&>("한국어"));
  RD_EXPECT_TRUE(enum_contains<Language>("English"));
  RD_EXPECT_TRUE(enum_contains<const Language>(lang));
  RD_EXPECT_TRUE(enum_contains<Language>("😃"));
  RD_EXPECT_FALSE(enum_contains<Language>("None"));
#endif

  constexpr auto nto = enum_contains<number>("three|one");
  RD_EXPECT_TRUE(enum_contains<number>("one"));
  RD_EXPECT_TRUE(enum_contains<number>("two"));
  RD_EXPECT_TRUE(enum_contains<number>("three"));
  RD_EXPECT_TRUE(enum_contains<number>("four"));
  RD_EXPECT_TRUE(nto);
  RD_EXPECT_FALSE(enum_contains<number>("None"));
}

RD_TEST(Flags, enum_value2) {
  constexpr auto cr = enum_value<Color>(0);
  RD_EXPECT_TRUE(cr == Color::RED);
  RD_EXPECT_TRUE(enum_value<Color &>(1) == Color::GREEN);
  RD_EXPECT_TRUE(enum_value<Color>(2) == Color::BLUE);

  RD_EXPECT_TRUE((enum_value<Color, 0>() == Color::RED));
  RD_EXPECT_TRUE((enum_value<Color, 1>() == Color::GREEN));
  RD_EXPECT_TRUE((enum_value<Color, 2>() == Color::BLUE));

  constexpr auto no = enum_value<Numbers>(0);
  RD_EXPECT_TRUE(no == Numbers::one);
  RD_EXPECT_TRUE(enum_value<Numbers>(1) == Numbers::two);
  RD_EXPECT_TRUE(enum_value<Numbers>(2) == Numbers::three);
  RD_EXPECT_TRUE(enum_value<Numbers>(3) == Numbers::many);

  RD_EXPECT_TRUE((enum_value<Numbers, 0>() == Numbers::one));
  RD_EXPECT_TRUE((enum_value<Numbers, 1>() == Numbers::two));
  RD_EXPECT_TRUE((enum_value<Numbers, 2>() == Numbers::three));
  RD_EXPECT_TRUE((enum_value<Numbers, 3>() == Numbers::many));

  constexpr auto dr = enum_value<Directions>(3);
  RD_EXPECT_TRUE(enum_value<Directions &>(0) == Directions::Left);
  RD_EXPECT_TRUE(enum_value<const Directions>(1) == Directions::Down);
  RD_EXPECT_TRUE(enum_value<Directions>(2) == Directions::Up);
  RD_EXPECT_TRUE(dr == Directions::Right);

  RD_EXPECT_TRUE((enum_value<Directions, 0>() == Directions::Left));
  RD_EXPECT_TRUE((enum_value<Directions, 1>() == Directions::Down));
  RD_EXPECT_TRUE((enum_value<Directions, 2>() == Directions::Up));
  RD_EXPECT_TRUE((enum_value<Directions, 3>() == Directions::Right));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto lang = enum_value<Language>(3);
  RD_EXPECT_TRUE(enum_value<Language&>(0) == Language::日本語);
  RD_EXPECT_TRUE(enum_value<const Language>(1) == Language::한국어);
  RD_EXPECT_TRUE(enum_value<Language>(2) == Language::English);
  RD_EXPECT_TRUE(lang == Language::😃);
#endif

  constexpr auto nt = enum_value<number>(2);
  RD_EXPECT_TRUE(enum_value<number>(0) == number::one);
  RD_EXPECT_TRUE(enum_value<number>(1) == number::two);
  RD_EXPECT_TRUE(nt == number::three);
  RD_EXPECT_TRUE(enum_value<number>(3) == number::four);

  RD_EXPECT_TRUE((enum_value<number, 0>() == number::one));
  RD_EXPECT_TRUE((enum_value<number, 1>() == number::two));
  RD_EXPECT_TRUE((enum_value<number, 2>() == number::three));
  RD_EXPECT_TRUE((enum_value<number, 3>() == number::four));
}

RD_TEST(Flags, enum_value3) {
  RD_EXPECT_TRUE((std::is_same_v<decltype(rendu::enum_values<Color>()), const std::array<Color, 3> &>));

  constexpr auto &s1 = enum_values<Color &>();
  RD_EXPECT_TRUE((s1 == std::array<Color, 3>{{Color::RED, Color::GREEN, Color::BLUE}}));

  constexpr auto &s2 = enum_values<Numbers>();
  RD_EXPECT_TRUE((s2 == std::array<Numbers, 4>{{Numbers::one, Numbers::two, Numbers::three, Numbers::many}}));

  constexpr auto &s3 = enum_values<const Directions>();
  RD_EXPECT_TRUE((s3 == std::array<Directions, 4>{{Directions::Left, Directions::Down, Directions::Up,
                                                   Directions::Right}}));

  constexpr auto &s4 = enum_values<number>();
  RD_EXPECT_TRUE((s4 == std::array<number, 4>{{number::one, number::two, number::three, number::four}}));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto& s5 = enum_values<const Language>();
  RD_EXPECT_TRUE(s5 == std::array<Language, 4>{{Language::日本語, Language::한국어, Language::English, Language::😃}});
#endif
}

RD_TEST(Flags, enum_count1) {
  constexpr auto s1 = enum_count<Color &>();
  RD_EXPECT_TRUE(s1 == 3);

  constexpr auto s2 = enum_count<Numbers>();
  RD_EXPECT_TRUE(s2 == 4);

  constexpr auto s3 = enum_count<const Directions>();
  RD_EXPECT_TRUE(s3 == 4);

  constexpr auto s4 = enum_count<number>();
  RD_EXPECT_TRUE(s4 == 4);

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto s5 = enum_count<Language>();
  RD_EXPECT_TRUE(s5 == 4);
#endif
}

RD_TEST(Flags, automatic_storage) {
  constexpr Color cr = Color::RED;
  constexpr auto cr_name = enum_name(cr);
  Color cm[3] = {Color::RED, Color::GREEN, Color::BLUE};
  Color cb = Color::BLUE;
  RD_EXPECT_TRUE(cr_name == "RED");
  RD_EXPECT_TRUE(enum_name<Color &>(cb) == "BLUE");
  RD_EXPECT_TRUE(enum_name(cm[1]) == "GREEN");
  RD_EXPECT_TRUE(enum_name(Color::RED | Color{0}) == "RED");
  RD_EXPECT_TRUE(enum_name(Color::RED | Color::GREEN).empty());
  RD_EXPECT_TRUE(enum_name(Color::RED | Color{8}).empty());
  RD_EXPECT_TRUE(enum_name(static_cast<Color>(0)).empty());

  constexpr Numbers no = Numbers::one;
  constexpr auto no_name = enum_name(no);
  RD_EXPECT_TRUE(no_name == "one");
  RD_EXPECT_TRUE(enum_name(Numbers::two) == "two");
  RD_EXPECT_TRUE(enum_name(Numbers::three) == "three");
  RD_EXPECT_TRUE(enum_name(Numbers::many) == "many");
  RD_EXPECT_TRUE(enum_name(Numbers::many | Numbers::two).empty());
  RD_EXPECT_TRUE(enum_name(static_cast<Numbers>(0)).empty());

  constexpr Directions dr = Directions::Right;
  constexpr auto dr_name = enum_name(dr);
  Directions du = Directions::Up;
  RD_EXPECT_TRUE(enum_name<Directions &>(du) == "Up");
  RD_EXPECT_TRUE(enum_name<const Directions>(Directions::Down) == "Down");
  RD_EXPECT_TRUE(dr_name == "Right");
  RD_EXPECT_TRUE(enum_name(Directions::Left) == "Left");
  RD_EXPECT_TRUE(enum_name(Directions::Right | Directions::Up | Directions::Left | Directions::Down).empty());
  RD_EXPECT_TRUE(enum_name(static_cast<Directions>(0)).empty());

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr Language lang = Language::日本語;
  constexpr auto lang_name = enum_name(lang);
  Language lk = Language::한국어;
  RD_EXPECT_TRUE(enum_name<Language&>(lk) == "한국어");
  RD_EXPECT_TRUE(enum_name<const Language>(Language::English) == "English");
  RD_EXPECT_TRUE(lang_name == "日本語");
  RD_EXPECT_TRUE(enum_name(Language::😃) == "😃");
  RD_EXPECT_TRUE(enum_name(static_cast<Language>(0)).empty());
#endif

  constexpr number nto = number::three | number::one;
  constexpr auto nto_name = enum_name(nto);
  RD_EXPECT_TRUE(enum_name(number::one) == "one");
  RD_EXPECT_TRUE(enum_name(number::two) == "two");
  RD_EXPECT_TRUE(enum_name(number::three) == "three");
  RD_EXPECT_TRUE(enum_name(number::four) == "four");
  RD_EXPECT_TRUE(nto_name.empty());
  RD_EXPECT_TRUE(enum_name(static_cast<number>(0)).empty());
}

RD_TEST(Flags, enum_flags_name) {
  constexpr Color cr = Color::RED;
  auto cr_name = enum_flags_name(cr);
  Color cm[3] = {Color::RED, Color::GREEN, Color::BLUE};
  Color cb = Color::BLUE;
  RD_EXPECT_TRUE(cr_name == "RED");
  RD_EXPECT_TRUE(enum_flags_name<Color &>(cb) == "BLUE");
  RD_EXPECT_TRUE(enum_flags_name(cm[1]) == "GREEN");
  RD_EXPECT_TRUE(enum_flags_name(Color::RED | Color{0}) == "RED");
  RD_EXPECT_TRUE(enum_flags_name(Color::RED | Color::GREEN) == "RED|GREEN");
  RD_EXPECT_TRUE(enum_flags_name(Color::RED | Color{8}).empty());
  RD_EXPECT_TRUE(enum_flags_name(static_cast<Color>(0)).empty());

  constexpr Numbers no = Numbers::one;
  auto no_name = enum_flags_name(no);
  RD_EXPECT_TRUE(no_name == "one");
  RD_EXPECT_TRUE(enum_flags_name(Numbers::two) == "two");
  RD_EXPECT_TRUE(enum_flags_name(Numbers::three) == "three");
  RD_EXPECT_TRUE(enum_flags_name(Numbers::many) == "many");
  RD_EXPECT_TRUE(enum_flags_name(Numbers::many | Numbers::two) == "two|many");
  RD_EXPECT_TRUE(enum_flags_name(static_cast<Numbers>(0)).empty());

  constexpr Directions dr = Directions::Right;
  auto dr_name = enum_flags_name(dr);
  Directions du = Directions::Up;
  RD_EXPECT_TRUE(enum_flags_name<Directions &>(du) == "Up");
  RD_EXPECT_TRUE(enum_flags_name<const Directions>(Directions::Down) == "Down");
  RD_EXPECT_TRUE(dr_name == "Right");
  RD_EXPECT_TRUE(enum_flags_name(Directions::Left) == "Left");
  RD_EXPECT_TRUE(enum_flags_name(Directions::Right | Directions::Up | Directions::Left | Directions::Down)
                     == "Left|Down|Up|Right");
  RD_EXPECT_TRUE(enum_flags_name(static_cast<Directions>(0)).empty());

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr Language lang = Language::日本語;
  auto lang_name = enum_flags_name(lang);
  Language lk = Language::한국어;
  RD_EXPECT_TRUE(enum_flags_name<Language&>(lk) == "한국어");
  RD_EXPECT_TRUE(enum_flags_name<const Language>(Language::English) == "English");
  RD_EXPECT_TRUE(lang_name == "日本語");
  RD_EXPECT_TRUE(enum_flags_name(Language::😃) == "😃");
  RD_EXPECT_TRUE(enum_flags_name(static_cast<Language>(0)).empty());
#endif

  constexpr number nto = number::three | number::one;
  auto nto_name = enum_flags_name(nto);
  RD_EXPECT_TRUE(enum_flags_name(number::one) == "one");
  RD_EXPECT_TRUE(enum_flags_name(number::two) == "two");
  RD_EXPECT_TRUE(enum_flags_name(number::three) == "three");
  RD_EXPECT_TRUE(enum_flags_name(number::four) == "four");
  RD_EXPECT_TRUE(nto_name == "one|three");
  RD_EXPECT_TRUE(enum_flags_name(static_cast<number>(0)).empty());
}

RD_TEST(Flags, enum_names2) {
  RD_EXPECT_TRUE((std::is_same_v<decltype(rendu::enum_names<Color>()), const std::array<std::string_view, 3> &>));

  constexpr auto &s1 = enum_names<Color &>();
  RD_EXPECT_TRUE((s1 == std::array<std::string_view, 3>{{"RED", "GREEN", "BLUE"}}));

  constexpr auto &s2 = enum_names<Numbers>();
  RD_EXPECT_TRUE((s2 == std::array<std::string_view, 4>{{"one", "two", "three", "many"}}));

  constexpr auto &s3 = enum_names<const Directions>();
  RD_EXPECT_TRUE((s3 == std::array<std::string_view, 4>{{"Left", "Down", "Up", "Right"}}));

  constexpr auto &s4 = enum_names<number>();
  RD_EXPECT_TRUE((s4 == std::array<std::string_view, 4>{{"one", "two", "three", "four"}}));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto& s5 = enum_names<const Language>();
  RD_EXPECT_TRUE(s5 == std::array<std::string_view, 4>{{"日本語", "한국어", "English", "😃"}});
#endif
}

RD_TEST(Flags, enum_entries2) {
  RD_EXPECT_TRUE((std::is_same_v<decltype(rendu::enum_entries<Color>()), const std::array<std::pair<Color,
                                                                                                    std::string_view>,
                                                                                          3> &>));

  constexpr auto &s1 = enum_entries<Color &>();
  RD_EXPECT_TRUE((s1 == std::array<std::pair<Color, std::string_view>, 3>{{{Color::RED, "RED"}, {Color::GREEN,
                                                                                                 "GREEN"},
                                                                           {Color::BLUE, "BLUE"}}}));

  constexpr auto &s2 = enum_entries<Numbers>();
  RD_EXPECT_TRUE((s2 == std::array<std::pair<Numbers, std::string_view>, 4>{{{Numbers::one, "one"}, {Numbers::two,
                                                                                                     "two"},
                                                                             {Numbers::three, "three"},
                                                                             {Numbers::many, "many"}}}));

  constexpr auto &s3 = enum_entries<Directions &>();
  RD_EXPECT_TRUE((s3 == std::array<std::pair<Directions, std::string_view>, 4>{{{Directions::Left, "Left"},
                                                                                {Directions::Down, "Down"},
                                                                                {Directions::Up, "Up"},
                                                                                {Directions::Right, "Right"}}}));

  constexpr auto &s4 = enum_entries<number>();
  RD_EXPECT_TRUE((s4 == std::array<std::pair<number, std::string_view>, 4>{{{number::one, "one"}, {number::two,
                                                                                                   "two"},
                                                                            {number::three, "three"},
                                                                            {number::four, "four"}}}));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto& s5 = enum_entries<const Language>();
  RD_EXPECT_EQ(s5 == std::array<std::pair<Language, std::string_view>, 4>{{{Language::日本語, "日本語"}, {Language::한국어, "한국어"}, {Language::English, "English"}, {Language::😃, "😃"}}});
#endif
}

RD_TEST(Flags, ostream_operators2) {
  auto test_ostream = [](auto e, std::string name) {
    using namespace rendu::ostream_operators;
    std::stringstream ss;
    ss << e;
    RD_EXPECT_TRUE(ss.str() == name);
  };

  test_ostream(std::make_optional(Color::RED), "RED");
  test_ostream(Color::GREEN, "GREEN");
  test_ostream(Color::BLUE, "BLUE");
  test_ostream(Color::BLUE | Color::RED, "RED|BLUE");
  test_ostream(Color::BLUE | Color::RED | Color::RED, "RED|BLUE");
  test_ostream(static_cast<Color>(0), "0");
  test_ostream(std::make_optional(static_cast<Color>(0)), "0");

  test_ostream(std::make_optional(Numbers::one), "one");
  test_ostream(Numbers::two, "two");
  test_ostream(Numbers::three, "three");
  test_ostream(Numbers::many, "many");
  test_ostream(static_cast<Numbers>(0), "0");
  test_ostream(std::make_optional(static_cast<Numbers>(0)), "0");

  test_ostream(std::make_optional(Directions::Up), "Up");
  test_ostream(Directions::Down, "Down");
  test_ostream(Directions::Right, "Right");
  test_ostream(Directions::Left, "Left");
  test_ostream(Directions::Right | Directions::Left, "Left|Right");
  test_ostream(static_cast<Directions>(0), "0");
  test_ostream(std::make_optional(static_cast<Directions>(0)), "0");

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  test_ostream(std::make_optional(Language::日本語), "日本語");
  test_ostream(Language::한국어, "한국어");
  test_ostream(Language::English, "English");
  test_ostream(Language::😃, "😃");
  test_ostream(static_cast<Language>(0), "0");
  test_ostream(std::make_optional(static_cast<Language>(0)), "0");
#endif

  test_ostream(std::make_optional(number::one), "one");
  test_ostream(number::two, "two");
  test_ostream(number::three, "three");
  test_ostream(number::four, "four");
  test_ostream(number::four | number::one, "one|four");
  test_ostream(static_cast<number>(0), "0");
  test_ostream(std::make_optional(static_cast<number>(0)), "0");
}

RD_TEST(Flags, istream_operators2) {
  auto test_istream = [](const auto e, std::string name) {
    using namespace rendu::istream_operators;
    std::istringstream ss(name);
    std::decay_t<decltype(e)> v;
    ss >> v;
    RD_EXPECT_TRUE(v == e);
    RD_EXPECT_TRUE(ss);
  };

  test_istream(Color::GREEN, "GREEN");
  test_istream(Color::BLUE, "BLUE");
  test_istream(Color::BLUE | Color::RED, "RED|BLUE");
  test_istream(Color::BLUE | Color::RED | Color::RED, "RED|BLUE");

  test_istream(Numbers::two, "two");
  test_istream(Numbers::three, "three");
  test_istream(Numbers::many, "many");

  test_istream(Directions::Down, "Down");
  test_istream(Directions::Right, "Right");
  test_istream(Directions::Left, "Left");
  test_istream(Directions::Right | Directions::Left, "Left|Right");

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  test_istream(Language::한국어, "한국어");
  test_istream(Language::English, "English");
  test_istream(Language::😃, "😃");
#endif

  test_istream(number::two, "two");
  test_istream(number::three, "three");
  test_istream(number::four, "four");
  test_istream(number::four | number::one, "one|four");
}

RD_TEST(Flags, bitwise_operator1) {
  RD_EXPECT_TRUE(enum_integer(~Color::RED) == ~enum_integer(Color::RED));
  RD_EXPECT_TRUE(enum_integer(~Numbers::one) == ~enum_integer(Numbers::one));
  RD_EXPECT_TRUE(enum_integer(~Directions::Up) == ~enum_integer(Directions::Up));
#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  RD_EXPECT_TRUE(enum_integer(~Language::日本語) == ~enum_integer(Language::日本語));
#endif
  RD_EXPECT_TRUE(enum_integer(~number::one) == ~enum_integer(number::one));
}
RD_TEST(Flags, bitwise_operator2) {
  RD_EXPECT_TRUE(enum_integer(Color::RED | Color::BLUE) == (enum_integer(Color::RED) | enum_integer(Color::BLUE)));
  RD_EXPECT_TRUE(
      enum_integer(Numbers::one | Numbers::two) == (enum_integer(Numbers::one) | enum_integer(Numbers::two)));
  RD_EXPECT_TRUE(enum_integer(Directions::Up | Directions::Down)
                     == (enum_integer(Directions::Up) | enum_integer(Directions::Down)));
#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  RD_EXPECT_TRUE(enum_integer(Language::日本語 | Language::한국어) == (enum_integer(Language::日本語) | enum_integer(Language::한국어)));
#endif
  RD_EXPECT_TRUE(enum_integer(number::one | number::two) == (enum_integer(number::one) | enum_integer(number::two)));
}
RD_TEST(Flags, bitwise_operator3) {

  RD_EXPECT_TRUE(enum_integer(Color::RED & Color::BLUE) == (enum_integer(Color::RED) & enum_integer(Color::BLUE)));
  RD_EXPECT_TRUE(
      enum_integer(Numbers::one & Numbers::two) == (enum_integer(Numbers::one) & enum_integer(Numbers::two)));
  RD_EXPECT_TRUE(enum_integer(Directions::Up & Directions::Down)
                     == (enum_integer(Directions::Up) & enum_integer(Directions::Down)));
#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  RD_EXPECT_TRUE(enum_integer(Language::日本語 & Language::한국어) == (enum_integer(Language::日本語) & enum_integer(Language::한국어)));
#endif
  RD_EXPECT_TRUE(enum_integer(number::one & number::two) == (enum_integer(number::one) & enum_integer(number::two)));
}

RD_TEST(Flags, bitwise_operator4) {
  RD_EXPECT_TRUE(enum_integer(Color::RED ^ Color::BLUE) == (enum_integer(Color::RED) ^ enum_integer(Color::BLUE)));
  RD_EXPECT_TRUE(
      enum_integer(Numbers::one ^ Numbers::two) == (enum_integer(Numbers::one) ^ enum_integer(Numbers::two)));
  RD_EXPECT_TRUE(enum_integer(Directions::Up ^ Directions::Down)
                     == (enum_integer(Directions::Up) ^ enum_integer(Directions::Down)));
#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  RD_EXPECT_TRUE(enum_integer(Language::日本語 ^ Language::한국어) == (enum_integer(Language::日本語) ^ enum_integer(Language::한국어)));
#endif
  RD_EXPECT_TRUE(enum_integer(number::one ^ number::two) == (enum_integer(number::one) ^ enum_integer(number::two)));
}

RD_TEST(Flags, bitwise_operator5) {
  Color x1 = Color::RED;
  x1 |= Color::BLUE;
  RD_EXPECT_TRUE(enum_integer(x1) == (enum_integer(Color::RED) | enum_integer(Color::BLUE)));

  Numbers x2 = Numbers::one;
  x2 |= Numbers::two;
  RD_EXPECT_TRUE(enum_integer(x2) == (enum_integer(Numbers::one) | enum_integer(Numbers::two)));

  Directions x3 = Directions::Up;
  x3 |= Directions::Down;
  RD_EXPECT_TRUE(enum_integer(x3) == (enum_integer(Directions::Up) | enum_integer(Directions::Down)));

  number x4 = number::one;
  x4 |= number::two;
  RD_EXPECT_TRUE(enum_integer(x4) == (enum_integer(number::one) | enum_integer(number::two)));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  Language x5 = Language::日本語;
  x5 |= Language::한국어;
  RD_EXPECT_TRUE(enum_integer(x5) == (enum_integer(Language::日本語) | enum_integer(Language::한국어)));
#endif
}

RD_TEST(Flags, bitwise_operator6) {
  Color x1 = Color::RED;
  x1 &= Color::BLUE;
  RD_EXPECT_TRUE(enum_integer(x1) == (enum_integer(Color::RED) & enum_integer(Color::BLUE)));

  Numbers x2 = Numbers::one;
  x2 &= Numbers::two;
  RD_EXPECT_TRUE(enum_integer(x2) == (enum_integer(Numbers::one) & enum_integer(Numbers::two)));

  Directions x3 = Directions::Up;
  x3 &= Directions::Down;
  RD_EXPECT_TRUE(enum_integer(x3) == (enum_integer(Directions::Up) & enum_integer(Directions::Down)));

  number x4 = number::one;
  x4 &= number::two;
  RD_EXPECT_TRUE(enum_integer(x4) == (enum_integer(number::one) & enum_integer(number::two)));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  Language x5 = Language::日本語;
  x5 &= Language::한국어;
  RD_EXPECT_TRUE(enum_integer(x5) == (enum_integer(Language::日本語) & enum_integer(Language::한국어)));
#endif
}

RD_TEST(Flags, bitwise_operator7) {
  Color x1 = Color::RED;
  x1 ^= Color::BLUE;
  RD_EXPECT_TRUE(enum_integer(x1) == (enum_integer(Color::RED) ^ enum_integer(Color::BLUE)));

  Numbers x2 = Numbers::one;
  x2 ^= Numbers::two;
  RD_EXPECT_TRUE(enum_integer(x2) == (enum_integer(Numbers::one) ^ enum_integer(Numbers::two)));

  Directions x3 = Directions::Up;
  x3 ^= Directions::Down;
  RD_EXPECT_TRUE(enum_integer(x3) == (enum_integer(Directions::Up) ^ enum_integer(Directions::Down)));

  number x4 = number::one;
  x4 ^= number::two;
  RD_EXPECT_TRUE(enum_integer(x4) == (enum_integer(number::one) ^ enum_integer(number::two)));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  Language x5 = Language::日本語;
  x5 ^= Language::한국어;
  RD_EXPECT_TRUE(enum_integer(x5) == (enum_integer(Language::日本語) ^ enum_integer(Language::한국어)));
#endif
}

#if defined(__clang__) && __clang_major__ >= 5 || defined(__GNUC__) && __GNUC__ >= 9 || defined(_MSC_VER) && _MSC_VER >= 1920
#  define MAGIC_ENUM_SUPPORTED_CONSTEXPR_FOR 1
#endif

#if defined(MAGIC_ENUM_SUPPORTED_CONSTEXPR_FOR)

// from https://artificial-mind.net/blog/2020/10/31/constexpr-for
template<auto Start, auto End, auto Inc, class F>
constexpr void constexpr_for(F &&f) {
  if constexpr (Start < End) {
    f(std::integral_constant<decltype(Start), Start>());
    constexpr_for<Start + Inc, End, Inc>(f);
  }
}

template<typename E, E V>
struct Foo {};

RD_TEST(Flags, constexpr_for2) {
  constexpr_for<0, rendu::enum_count<Color>(), 1>([](auto i) {
    [[maybe_unused]] Foo<Color, rendu::enum_value<Color, i>()> bar{};
  });
}

#endif

#if defined(__cpp_lib_format)

#include <rendu_format.hpp>

RD_TEST("format-base") {
  RD_EXPECT_TRUE(std::format("Test-{:~^11}.", Color::RED | Color::GREEN) == "Test-~RED|GREEN~.");
}

#endif
}
