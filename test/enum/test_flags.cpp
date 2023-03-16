/*
* Created by boil on 2022/9/25.
*/
#include <test/rdtest.h>
#include "enum_fwd.h"
using namespace magic_enum;
using namespace magic_enum::bitwise_operators;

TEST(Flags, enum_string) {
  constexpr auto cr = enum_cast<Color>("RED");
  EXPECT_TRUE(cr.value() == Color::RED);
  EXPECT_TRUE(enum_cast<Color &>("GREEN").value() == Color::GREEN);
  EXPECT_TRUE(
      enum_cast<Color>("blue", [](char lhs, char rhs) { return std::tolower(lhs) == std::tolower(rhs); }).value()
          == Color::BLUE);
  EXPECT_TRUE(enum_cast<Color &>("blue|RED",
                                    [](char lhs, char rhs) { return std::tolower(lhs) == std::tolower(rhs); }).value()
                     == (Color::BLUE | Color::RED));
  EXPECT_TRUE(enum_cast<Color &>("GREEN|RED").value() == (Color::GREEN | Color::RED));
  EXPECT_TRUE(enum_cast<Color &>("GREEN|RED|RED").value() == (Color::GREEN | Color::RED));
  EXPECT_FALSE(enum_cast<Color &>("GREEN|RED|None").has_value());
  EXPECT_FALSE(enum_cast<Color>("None").has_value());

  constexpr auto no = enum_cast<Numbers>("one");
  EXPECT_TRUE(no.value() == Numbers::one);
  EXPECT_TRUE(enum_cast<Numbers>("two").value() == Numbers::two);
  EXPECT_TRUE(enum_cast<Numbers>("three").value() == Numbers::three);
  EXPECT_TRUE(enum_cast<Numbers>("many") == Numbers::many);
  EXPECT_FALSE(enum_cast<Numbers>("None").has_value());

  constexpr auto dr = enum_cast<Directions>("Right");
  EXPECT_TRUE(enum_cast<Directions &>("Up").value() == Directions::Up);
  EXPECT_TRUE(enum_cast<const Directions>("Down").value() == Directions::Down);
  EXPECT_TRUE(dr.value() == Directions::Right);
  EXPECT_TRUE(enum_cast<Directions>("Left").value() == Directions::Left);
  EXPECT_FALSE(enum_cast<Directions>("None").has_value());

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto lang = enum_cast<Language>("日本語");
  EXPECT_TRUE(enum_cast<Language&>("한국어").value() == Language::한국어);
  EXPECT_TRUE(enum_cast<const Language>("English").value() == Language::English);
  EXPECT_TRUE(lang.value() == Language::日本語);
  EXPECT_TRUE(enum_cast<Language>("😃").value() == Language::😃);
  EXPECT_FALSE(enum_cast<Language>("None").has_value());
#endif

  constexpr auto nto = enum_cast<number>("three|one");
  EXPECT_TRUE(enum_cast<number>("one").value() == number::one);
  EXPECT_TRUE(enum_cast<number>("two").value() == number::two);
  EXPECT_TRUE(enum_cast<number>("three").value() == number::three);
  EXPECT_TRUE(enum_cast<number>("four") == number::four);
  EXPECT_TRUE(nto.value() == (number::three | number::one));
  EXPECT_FALSE(enum_cast<number>("None").has_value());
}

TEST(Flags, enum_integer) {
  Color cm[3] = {Color::RED, Color::GREEN, Color::BLUE};
  constexpr auto cr = enum_cast<Color>(1);
  EXPECT_TRUE(cr.value() == Color::RED);
  EXPECT_TRUE(enum_cast<Color &>(2).value() == Color::GREEN);
  EXPECT_TRUE(enum_cast<Color>(static_cast<int>(cm[2])).value() == Color::BLUE);
  EXPECT_TRUE(enum_cast<Color>(1 | 2).value() == (Color::GREEN | Color::RED));
  EXPECT_TRUE(enum_cast<Color>(1 | 2 | 1).value() == (Color::GREEN | Color::RED));
  EXPECT_FALSE(enum_cast<Color>(1 | 2 | 8).has_value());
  EXPECT_FALSE(enum_cast<Color>(0).has_value());

  constexpr auto no = enum_cast<Numbers>(2);
  EXPECT_TRUE(no.value() == Numbers::one);
  EXPECT_TRUE(enum_cast<Numbers>(4).value() == Numbers::two);
  EXPECT_TRUE(enum_cast<Numbers>(8).value() == Numbers::three);
  EXPECT_TRUE(enum_cast<Numbers>(1 << 30).value() == Numbers::many);
  EXPECT_FALSE(enum_cast<Numbers>(127).has_value());
  EXPECT_FALSE(enum_cast<Numbers>(0).has_value());

  constexpr auto dr = enum_cast<Directions>(std::uint64_t{1} << 63);
  EXPECT_TRUE(enum_cast<Directions &>(std::uint64_t{1} << 31).value() == Directions::Up);
  EXPECT_TRUE(enum_cast<const Directions>(std::uint64_t{1} << 20).value() == Directions::Down);
  EXPECT_TRUE(dr.value() == Directions::Right);
  EXPECT_TRUE(enum_cast<Directions>(std::uint64_t{1} << 10).value() == Directions::Left);
  EXPECT_FALSE(enum_cast<Directions>(0).has_value());

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto lang = enum_cast<Language>(1 << 1);
  EXPECT_TRUE(enum_cast<Language&>(1 << 2).value() == Language::한국어);
  EXPECT_TRUE(enum_cast<const Language>(1 << 3).value() == Language::English);
  EXPECT_TRUE(lang.value() == Language::日本語);
  EXPECT_TRUE(enum_cast<Language>(1 << 4).value() == Language::😃);
  EXPECT_FALSE(enum_cast<Language>(0).has_value());
#endif

  constexpr auto nto = enum_cast<number>(2 | 8);
  EXPECT_TRUE(enum_cast<number>(1 << 1).value() == number::one);
  EXPECT_TRUE(enum_cast<number>(1 << 2).value() == number::two);
  EXPECT_TRUE(enum_cast<number>(1 << 3).value() == number::three);
  EXPECT_TRUE(enum_cast<number>(1 << 4).value() == number::four);
  EXPECT_TRUE(nto.value() == (number::three | number::one));
  EXPECT_FALSE(enum_cast<number>(0).has_value());
}

TEST(Flags, enum_index2) {
  Color cm[3] = {Color::RED, Color::GREEN, Color::BLUE};
  constexpr auto cr = enum_index(Color::RED);
  Color cg = Color::GREEN;
  EXPECT_TRUE(cr.value() == 0);
  EXPECT_TRUE(enum_index<Color &>(cg).value() == 1);
  EXPECT_TRUE(enum_index(cm[2]).value() == 2);
  EXPECT_FALSE(enum_index<Color>(Color::RED | Color::GREEN).has_value());
  EXPECT_FALSE(enum_index<Color>(Color::RED | Color::GREEN | Color::RED).has_value());
  EXPECT_FALSE(enum_index<Color>(Color::RED | Color{8}).has_value());
  EXPECT_FALSE(enum_index(static_cast<Color>(0)).has_value());

  constexpr auto no = enum_index(Numbers::one);
  EXPECT_TRUE(no.value() == 0);
  EXPECT_TRUE(enum_index(Numbers::two).value() == 1);
  EXPECT_TRUE(enum_index(Numbers::three).value() == 2);
  EXPECT_TRUE(enum_index(Numbers::many).value() == 3);
  EXPECT_FALSE(enum_index(static_cast<Numbers>(0)).has_value());

  constexpr auto dr = enum_index(Directions::Right);
  Directions dl = Directions::Left;
  EXPECT_TRUE(enum_index<Directions &>(dl).value() == 0);
  EXPECT_TRUE(enum_index<const Directions>(Directions::Down).value() == 1);
  EXPECT_TRUE(enum_index(Directions::Up).value() == 2);
  EXPECT_TRUE(dr.value() == 3);
  EXPECT_FALSE(enum_index(static_cast<Directions>(0)).has_value());

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto lang = enum_index<Language>(Language::日本語);
  Language korean = Language::한국어;
  EXPECT_TRUE(enum_index<Language&>(korean).value() == 1);
  EXPECT_TRUE(enum_index<const Language>(Language::English).value() == 2);
  EXPECT_TRUE(enum_index(Language::😃).value() == 3);
  EXPECT_TRUE(lang.value() == 0);
  EXPECT_FALSE(enum_index(static_cast<Language>(0)).has_value());
#endif

  constexpr auto nto = enum_index(number::three | number::one);
  EXPECT_TRUE(enum_index(number::one).value() == 0);
  EXPECT_TRUE(enum_index(number::two).value() == 1);
  EXPECT_TRUE(enum_index(number::three).value() == 2);
  EXPECT_TRUE(enum_index(number::four).value() == 3);
  EXPECT_FALSE(nto.has_value());
  EXPECT_FALSE(enum_index(static_cast<number>(0)).has_value());
}

TEST(Flags, enum_contains_value) {
  Color cm[3] = {Color::RED, Color::GREEN, Color::BLUE};
  constexpr auto cr = enum_contains(Color::RED);
  Color cg = Color::GREEN;
  EXPECT_TRUE(cr);
  EXPECT_TRUE(enum_contains<Color &>(cg));
  EXPECT_TRUE(enum_contains(cm[2]));
  EXPECT_TRUE(enum_contains<Color>(Color::RED | Color::GREEN));
  EXPECT_TRUE(enum_contains<Color>(Color::RED | Color::GREEN | Color::GREEN));
  EXPECT_FALSE(enum_contains<Color>(Color::RED | Color{8}));
  EXPECT_FALSE(enum_contains(static_cast<Color>(0)));

  constexpr auto no = enum_contains(Numbers::one);
  EXPECT_TRUE(no);
  EXPECT_TRUE(enum_contains(Numbers::two));
  EXPECT_TRUE(enum_contains(Numbers::three));
  EXPECT_TRUE(enum_contains(Numbers::many));
  EXPECT_FALSE(enum_contains(static_cast<Numbers>(0)));

  constexpr auto dr = enum_contains(Directions::Right);
  Directions dl = Directions::Left;
  EXPECT_TRUE(enum_contains<Directions &>(dl));
  EXPECT_TRUE(enum_contains<const Directions>(Directions::Down));
  EXPECT_TRUE(enum_contains(Directions::Up));
  EXPECT_TRUE(dr);
  EXPECT_FALSE(enum_contains(static_cast<Directions>(0)));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto lang = enum_index<Language>(Language::日本語);
  Language korean = Language::한국어;
  EXPECT_TRUE(enum_contains<Language&>(korean));
  EXPECT_TRUE(enum_contains<const Language>(Language::English));
  EXPECT_TRUE(enum_contains(Language::😃));
  EXPECT_TRUE(lang);
  EXPECT_FALSE(enum_contains(static_cast<Language>(0)));
#endif

  constexpr auto nto = enum_contains(number::three | number::one);
  EXPECT_TRUE(enum_contains(number::one));
  EXPECT_TRUE(enum_contains<number &>(number::two));
  EXPECT_TRUE(enum_contains(number::one));
  EXPECT_TRUE(enum_contains(number::four));
  EXPECT_TRUE(nto);
  EXPECT_FALSE(enum_contains(static_cast<number>(0)));
}

TEST(Flags, enum_contains_integer) {
  EXPECT_TRUE(enum_contains<Color>(1));
  EXPECT_TRUE(enum_contains<Color &>(2));
  EXPECT_TRUE(enum_contains<Color>(4));
  EXPECT_TRUE(enum_contains<Color>(1 | 2));
  EXPECT_TRUE(enum_contains<Color>(1 | 2 | 1));
  EXPECT_FALSE(enum_contains<Color>(1 | 2 | 8));
  EXPECT_FALSE(enum_contains<Color>(0));

  constexpr auto no = enum_contains<Numbers>(1 << 1);
  EXPECT_TRUE(no);
  EXPECT_TRUE(enum_contains<Numbers>(1 << 2));
  EXPECT_TRUE(enum_contains<Numbers>(1 << 3));
  EXPECT_TRUE(enum_contains<Numbers>(1 << 30));

  constexpr auto dr = enum_contains<Directions &>(std::uint64_t{1} << 63);
  EXPECT_TRUE(dr);
  EXPECT_TRUE(enum_contains<const Directions>(std::uint64_t{1} << 10));
  EXPECT_TRUE(enum_contains<Directions>(std::uint64_t{1} << 20));
  EXPECT_TRUE(enum_contains<Directions>(std::uint64_t{1} << 31));
  EXPECT_FALSE(enum_contains<Directions>(static_cast<Directions>(0)));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto lang = enum_contains<Language&>(1 << 1);
  EXPECT_TRUE(lang);
  EXPECT_TRUE(enum_contains<const Language>(1 << 2));
  EXPECT_TRUE(enum_contains<Language>(1 << 3));
  EXPECT_TRUE(enum_contains<Language>(1 << 4));
  EXPECT_FALSE(enum_contains(static_cast<Language>(0)));
#endif

  constexpr auto nto = enum_contains<number>(8 | 2);
  EXPECT_TRUE(enum_contains<number>(1 << 1));
  EXPECT_TRUE(enum_contains<number>(1 << 2));
  EXPECT_TRUE(enum_contains<number>(1 << 3));
  EXPECT_TRUE(enum_contains<number>(1 << 4));
  EXPECT_TRUE(enum_contains<number>(8 | 2 | 16));
  EXPECT_TRUE(enum_contains<number>(8 | 16 | 16));
  EXPECT_TRUE(nto);
  EXPECT_FALSE(enum_contains<number>(8 | 64));
  EXPECT_FALSE(enum_contains<number>(0));
}

TEST(Flags, enum_contains_string) {
  constexpr auto cr = "RED";
  EXPECT_TRUE(enum_contains<Color>(cr));
  EXPECT_TRUE(enum_contains<Color &>("GREEN"));
  EXPECT_TRUE(enum_contains<Color>("blue",
                                      [](char lhs, char rhs) { return std::tolower(lhs) == std::tolower(rhs); }));
  EXPECT_TRUE(enum_contains<Color &>("blue|RED",
                                        [](char lhs, char rhs) { return std::tolower(lhs) == std::tolower(rhs); }));
  EXPECT_TRUE(enum_contains<Color &>("GREEN|RED"));
  EXPECT_TRUE(enum_contains<Color &>("GREEN|RED|RED"));
  EXPECT_FALSE(enum_contains<Color>("GREEN|RED|None"));
  EXPECT_FALSE(enum_contains<Color>("None"));

  constexpr auto no = std::string_view{"one"};
  EXPECT_TRUE(enum_contains<Numbers>(no));
  EXPECT_TRUE(enum_contains<Numbers>("two"));
  EXPECT_TRUE(enum_contains<Numbers>("three"));
  EXPECT_TRUE(enum_contains<Numbers>("many"));
  EXPECT_FALSE(enum_contains<Numbers>("None"));

  auto dr = std::string{"Right"};
  EXPECT_TRUE(enum_contains<Directions &>("Up"));
  EXPECT_TRUE(enum_contains<Directions>("Down"));
  EXPECT_TRUE(enum_contains<const Directions>(dr));
  EXPECT_TRUE(enum_contains<Directions>("Left"));
  EXPECT_FALSE(enum_contains<Directions>("None"));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  auto lang = std::string{"日本語"};
  EXPECT_TRUE(enum_contains<Language&>("한국어"));
  EXPECT_TRUE(enum_contains<Language>("English"));
  EXPECT_TRUE(enum_contains<const Language>(lang));
  EXPECT_TRUE(enum_contains<Language>("😃"));
  EXPECT_FALSE(enum_contains<Language>("None"));
#endif

  constexpr auto nto = enum_contains<number>("three|one");
  EXPECT_TRUE(enum_contains<number>("one"));
  EXPECT_TRUE(enum_contains<number>("two"));
  EXPECT_TRUE(enum_contains<number>("three"));
  EXPECT_TRUE(enum_contains<number>("four"));
  EXPECT_TRUE(nto);
  EXPECT_FALSE(enum_contains<number>("None"));
}

TEST(Flags, enum_value2) {
  constexpr auto cr = enum_value<Color>(0);
  EXPECT_TRUE(cr == Color::RED);
  EXPECT_TRUE(enum_value<Color &>(1) == Color::GREEN);
  EXPECT_TRUE(enum_value<Color>(2) == Color::BLUE);

  EXPECT_TRUE((enum_value<Color, 0>() == Color::RED));
  EXPECT_TRUE((enum_value<Color, 1>() == Color::GREEN));
  EXPECT_TRUE((enum_value<Color, 2>() == Color::BLUE));

  constexpr auto no = enum_value<Numbers>(0);
  EXPECT_TRUE(no == Numbers::one);
  EXPECT_TRUE(enum_value<Numbers>(1) == Numbers::two);
  EXPECT_TRUE(enum_value<Numbers>(2) == Numbers::three);
  EXPECT_TRUE(enum_value<Numbers>(3) == Numbers::many);

  EXPECT_TRUE((enum_value<Numbers, 0>() == Numbers::one));
  EXPECT_TRUE((enum_value<Numbers, 1>() == Numbers::two));
  EXPECT_TRUE((enum_value<Numbers, 2>() == Numbers::three));
  EXPECT_TRUE((enum_value<Numbers, 3>() == Numbers::many));

  constexpr auto dr = enum_value<Directions>(3);
  EXPECT_TRUE(enum_value<Directions &>(0) == Directions::Left);
  EXPECT_TRUE(enum_value<const Directions>(1) == Directions::Down);
  EXPECT_TRUE(enum_value<Directions>(2) == Directions::Up);
  EXPECT_TRUE(dr == Directions::Right);

  EXPECT_TRUE((enum_value<Directions, 0>() == Directions::Left));
  EXPECT_TRUE((enum_value<Directions, 1>() == Directions::Down));
  EXPECT_TRUE((enum_value<Directions, 2>() == Directions::Up));
  EXPECT_TRUE((enum_value<Directions, 3>() == Directions::Right));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto lang = enum_value<Language>(3);
  EXPECT_TRUE(enum_value<Language&>(0) == Language::日本語);
  EXPECT_TRUE(enum_value<const Language>(1) == Language::한국어);
  EXPECT_TRUE(enum_value<Language>(2) == Language::English);
  EXPECT_TRUE(lang == Language::😃);
#endif

  constexpr auto nt = enum_value<number>(2);
  EXPECT_TRUE(enum_value<number>(0) == number::one);
  EXPECT_TRUE(enum_value<number>(1) == number::two);
  EXPECT_TRUE(nt == number::three);
  EXPECT_TRUE(enum_value<number>(3) == number::four);

  EXPECT_TRUE((enum_value<number, 0>() == number::one));
  EXPECT_TRUE((enum_value<number, 1>() == number::two));
  EXPECT_TRUE((enum_value<number, 2>() == number::three));
  EXPECT_TRUE((enum_value<number, 3>() == number::four));
}

TEST(Flags, enum_value3) {
  EXPECT_TRUE((std::is_same_v<decltype(enum_values<Color>()), const std::array<Color, 3> &>));

  constexpr auto &s1 = enum_values<Color &>();
  EXPECT_TRUE((s1 == std::array<Color, 3>{{Color::RED, Color::GREEN, Color::BLUE}}));

  constexpr auto &s2 = enum_values<Numbers>();
  EXPECT_TRUE((s2 == std::array<Numbers, 4>{{Numbers::one, Numbers::two, Numbers::three, Numbers::many}}));

  constexpr auto &s3 = enum_values<const Directions>();
  EXPECT_TRUE((s3 == std::array<Directions, 4>{{Directions::Left, Directions::Down, Directions::Up,
                                                   Directions::Right}}));

  constexpr auto &s4 = enum_values<number>();
  EXPECT_TRUE((s4 == std::array<number, 4>{{number::one, number::two, number::three, number::four}}));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto& s5 = enum_values<const Language>();
  EXPECT_TRUE(s5 == std::array<Language, 4>{{Language::日本語, Language::한국어, Language::English, Language::😃}});
#endif
}

TEST(Flags, enum_count1) {
  constexpr auto s1 = enum_count<Color &>();
  EXPECT_TRUE(s1 == 3);

  constexpr auto s2 = enum_count<Numbers>();
  EXPECT_TRUE(s2 == 4);

  constexpr auto s3 = enum_count<const Directions>();
  EXPECT_TRUE(s3 == 4);

  constexpr auto s4 = enum_count<number>();
  EXPECT_TRUE(s4 == 4);

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto s5 = enum_count<Language>();
  EXPECT_TRUE(s5 == 4);
#endif
}

TEST(Flags, automatic_storage) {
  constexpr Color cr = Color::RED;
  constexpr auto cr_name = enum_name(cr);
  Color cm[3] = {Color::RED, Color::GREEN, Color::BLUE};
  Color cb = Color::BLUE;
  EXPECT_TRUE(cr_name == "RED");
  EXPECT_TRUE(enum_name<Color &>(cb) == "BLUE");
  EXPECT_TRUE(enum_name(cm[1]) == "GREEN");
  EXPECT_TRUE(enum_name(Color::RED | Color{0}) == "RED");
  EXPECT_TRUE(enum_name(Color::RED | Color::GREEN).empty());
  EXPECT_TRUE(enum_name(Color::RED | Color{8}).empty());
  EXPECT_TRUE(enum_name(static_cast<Color>(0)).empty());

  constexpr Numbers no = Numbers::one;
  constexpr auto no_name = enum_name(no);
  EXPECT_TRUE(no_name == "one");
  EXPECT_TRUE(enum_name(Numbers::two) == "two");
  EXPECT_TRUE(enum_name(Numbers::three) == "three");
  EXPECT_TRUE(enum_name(Numbers::many) == "many");
  EXPECT_TRUE(enum_name(Numbers::many | Numbers::two).empty());
  EXPECT_TRUE(enum_name(static_cast<Numbers>(0)).empty());

  constexpr Directions dr = Directions::Right;
  constexpr auto dr_name = enum_name(dr);
  Directions du = Directions::Up;
  EXPECT_TRUE(enum_name<Directions &>(du) == "Up");
  EXPECT_TRUE(enum_name<const Directions>(Directions::Down) == "Down");
  EXPECT_TRUE(dr_name == "Right");
  EXPECT_TRUE(enum_name(Directions::Left) == "Left");
  EXPECT_TRUE(enum_name(Directions::Right | Directions::Up | Directions::Left | Directions::Down).empty());
  EXPECT_TRUE(enum_name(static_cast<Directions>(0)).empty());

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr Language lang = Language::日本語;
  constexpr auto lang_name = enum_name(lang);
  Language lk = Language::한국어;
  EXPECT_TRUE(enum_name<Language&>(lk) == "한국어");
  EXPECT_TRUE(enum_name<const Language>(Language::English) == "English");
  EXPECT_TRUE(lang_name == "日本語");
  EXPECT_TRUE(enum_name(Language::😃) == "😃");
  EXPECT_TRUE(enum_name(static_cast<Language>(0)).empty());
#endif

  constexpr number nto = number::three | number::one;
  constexpr auto nto_name = enum_name(nto);
  EXPECT_TRUE(enum_name(number::one) == "one");
  EXPECT_TRUE(enum_name(number::two) == "two");
  EXPECT_TRUE(enum_name(number::three) == "three");
  EXPECT_TRUE(enum_name(number::four) == "four");
  EXPECT_TRUE(nto_name.empty());
  EXPECT_TRUE(enum_name(static_cast<number>(0)).empty());
}

TEST(Flags, enum_flags_name) {
  constexpr Color cr = Color::RED;
  auto cr_name = enum_flags_name(cr);
  Color cm[3] = {Color::RED, Color::GREEN, Color::BLUE};
  Color cb = Color::BLUE;
  EXPECT_TRUE(cr_name == "RED");
  EXPECT_TRUE(enum_flags_name<Color &>(cb) == "BLUE");
  EXPECT_TRUE(enum_flags_name(cm[1]) == "GREEN");
  EXPECT_TRUE(enum_flags_name(Color::RED | Color{0}) == "RED");
  EXPECT_TRUE(enum_flags_name(Color::RED | Color::GREEN) == "RED|GREEN");
  EXPECT_TRUE(enum_flags_name(Color::RED | Color{8}).empty());
  EXPECT_TRUE(enum_flags_name(static_cast<Color>(0)).empty());

  constexpr Numbers no = Numbers::one;
  auto no_name = enum_flags_name(no);
  EXPECT_TRUE(no_name == "one");
  EXPECT_TRUE(enum_flags_name(Numbers::two) == "two");
  EXPECT_TRUE(enum_flags_name(Numbers::three) == "three");
  EXPECT_TRUE(enum_flags_name(Numbers::many) == "many");
  EXPECT_TRUE(enum_flags_name(Numbers::many | Numbers::two) == "two|many");
  EXPECT_TRUE(enum_flags_name(static_cast<Numbers>(0)).empty());

  constexpr Directions dr = Directions::Right;
  auto dr_name = enum_flags_name(dr);
  Directions du = Directions::Up;
  EXPECT_TRUE(enum_flags_name<Directions &>(du) == "Up");
  EXPECT_TRUE(enum_flags_name<const Directions>(Directions::Down) == "Down");
  EXPECT_TRUE(dr_name == "Right");
  EXPECT_TRUE(enum_flags_name(Directions::Left) == "Left");
  EXPECT_TRUE(enum_flags_name(Directions::Right | Directions::Up | Directions::Left | Directions::Down)
                     == "Left|Down|Up|Right");
  EXPECT_TRUE(enum_flags_name(static_cast<Directions>(0)).empty());

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr Language lang = Language::日本語;
  auto lang_name = enum_flags_name(lang);
  Language lk = Language::한국어;
  EXPECT_TRUE(enum_flags_name<Language&>(lk) == "한국어");
  EXPECT_TRUE(enum_flags_name<const Language>(Language::English) == "English");
  EXPECT_TRUE(lang_name == "日本語");
  EXPECT_TRUE(enum_flags_name(Language::😃) == "😃");
  EXPECT_TRUE(enum_flags_name(static_cast<Language>(0)).empty());
#endif

  constexpr number nto = number::three | number::one;
  auto nto_name = enum_flags_name(nto);
  EXPECT_TRUE(enum_flags_name(number::one) == "one");
  EXPECT_TRUE(enum_flags_name(number::two) == "two");
  EXPECT_TRUE(enum_flags_name(number::three) == "three");
  EXPECT_TRUE(enum_flags_name(number::four) == "four");
  EXPECT_TRUE(nto_name == "one|three");
  EXPECT_TRUE(enum_flags_name(static_cast<number>(0)).empty());
}

TEST(Flags, enum_names2) {
  EXPECT_TRUE((std::is_same_v<decltype(enum_names<Color>()), const std::array<std::string_view, 3> &>));

  constexpr auto &s1 = enum_names<Color &>();
  EXPECT_TRUE((s1 == std::array<std::string_view, 3>{{"RED", "GREEN", "BLUE"}}));

  constexpr auto &s2 = enum_names<Numbers>();
  EXPECT_TRUE((s2 == std::array<std::string_view, 4>{{"one", "two", "three", "many"}}));

  constexpr auto &s3 = enum_names<const Directions>();
  EXPECT_TRUE((s3 == std::array<std::string_view, 4>{{"Left", "Down", "Up", "Right"}}));

  constexpr auto &s4 = enum_names<number>();
  EXPECT_TRUE((s4 == std::array<std::string_view, 4>{{"one", "two", "three", "four"}}));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto& s5 = enum_names<const Language>();
  EXPECT_TRUE(s5 == std::array<std::string_view, 4>{{"日本語", "한국어", "English", "😃"}});
#endif
}

TEST(Flags, enum_entries2) {
  EXPECT_TRUE((std::is_same_v<decltype(enum_entries<Color>()), const std::array<std::pair<Color,
                                                                                                    std::string_view>,
                                                                                          3> &>));

  constexpr auto &s1 = enum_entries<Color &>();
  EXPECT_TRUE((s1 == std::array<std::pair<Color, std::string_view>, 3>{{{Color::RED, "RED"}, {Color::GREEN,
                                                                                                 "GREEN"},
                                                                           {Color::BLUE, "BLUE"}}}));

  constexpr auto &s2 = enum_entries<Numbers>();
  EXPECT_TRUE((s2 == std::array<std::pair<Numbers, std::string_view>, 4>{{{Numbers::one, "one"}, {Numbers::two,
                                                                                                     "two"},
                                                                             {Numbers::three, "three"},
                                                                             {Numbers::many, "many"}}}));

  constexpr auto &s3 = enum_entries<Directions &>();
  EXPECT_TRUE((s3 == std::array<std::pair<Directions, std::string_view>, 4>{{{Directions::Left, "Left"},
                                                                                {Directions::Down, "Down"},
                                                                                {Directions::Up, "Up"},
                                                                                {Directions::Right, "Right"}}}));

  constexpr auto &s4 = enum_entries<number>();
  EXPECT_TRUE((s4 == std::array<std::pair<number, std::string_view>, 4>{{{number::one, "one"}, {number::two,
                                                                                                   "two"},
                                                                            {number::three, "three"},
                                                                            {number::four, "four"}}}));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  constexpr auto& s5 = enum_entries<const Language>();
  RD_EXPECT_EQ(s5 == std::array<std::pair<Language, std::string_view>, 4>{{{Language::日本語, "日本語"}, {Language::한국어, "한국어"}, {Language::English, "English"}, {Language::😃, "😃"}}});
#endif
}

TEST(Flags, ostream_operators2) {
  auto test_ostream = [](auto e, std::string name) {
    using namespace ostream_operators;
    std::stringstream ss;
    ss << e;
    EXPECT_TRUE(ss.str() == name);
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

TEST(Flags, istream_operators2) {
  auto test_istream = [](const auto e, std::string name) {
    using namespace istream_operators;
    std::istringstream ss(name);
    std::decay_t<decltype(e)> v;
    ss >> v;
    EXPECT_TRUE(v == e);
    EXPECT_TRUE(ss);
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

TEST(Flags, bitwise_operator1) {
  EXPECT_TRUE(enum_integer(~Color::RED) == ~enum_integer(Color::RED));
  EXPECT_TRUE(enum_integer(~Numbers::one) == ~enum_integer(Numbers::one));
  EXPECT_TRUE(enum_integer(~Directions::Up) == ~enum_integer(Directions::Up));
#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  EXPECT_TRUE(enum_integer(~Language::日本語) == ~enum_integer(Language::日本語));
#endif
  EXPECT_TRUE(enum_integer(~number::one) == ~enum_integer(number::one));
}
TEST(Flags, bitwise_operator2) {
  EXPECT_TRUE(enum_integer(Color::RED | Color::BLUE) == (enum_integer(Color::RED) | enum_integer(Color::BLUE)));
  EXPECT_TRUE(
      enum_integer(Numbers::one | Numbers::two) == (enum_integer(Numbers::one) | enum_integer(Numbers::two)));
  EXPECT_TRUE(enum_integer(Directions::Up | Directions::Down)
                     == (enum_integer(Directions::Up) | enum_integer(Directions::Down)));
#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  EXPECT_TRUE(enum_integer(Language::日本語 | Language::한국어) == (enum_integer(Language::日本語) | enum_integer(Language::한국어)));
#endif
  EXPECT_TRUE(enum_integer(number::one | number::two) == (enum_integer(number::one) | enum_integer(number::two)));
}
TEST(Flags, bitwise_operator3) {

  EXPECT_TRUE(enum_integer(Color::RED & Color::BLUE) == (enum_integer(Color::RED) & enum_integer(Color::BLUE)));
  EXPECT_TRUE(
      enum_integer(Numbers::one & Numbers::two) == (enum_integer(Numbers::one) & enum_integer(Numbers::two)));
  EXPECT_TRUE(enum_integer(Directions::Up & Directions::Down)
                     == (enum_integer(Directions::Up) & enum_integer(Directions::Down)));
#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  EXPECT_TRUE(enum_integer(Language::日本語 & Language::한국어) == (enum_integer(Language::日本語) & enum_integer(Language::한국어)));
#endif
  EXPECT_TRUE(enum_integer(number::one & number::two) == (enum_integer(number::one) & enum_integer(number::two)));
}

TEST(Flags, bitwise_operator4) {
  EXPECT_TRUE(enum_integer(Color::RED ^ Color::BLUE) == (enum_integer(Color::RED) ^ enum_integer(Color::BLUE)));
  EXPECT_TRUE(
      enum_integer(Numbers::one ^ Numbers::two) == (enum_integer(Numbers::one) ^ enum_integer(Numbers::two)));
  EXPECT_TRUE(enum_integer(Directions::Up ^ Directions::Down)
                     == (enum_integer(Directions::Up) ^ enum_integer(Directions::Down)));
#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  EXPECT_TRUE(enum_integer(Language::日本語 ^ Language::한국어) == (enum_integer(Language::日本語) ^ enum_integer(Language::한국어)));
#endif
  EXPECT_TRUE(enum_integer(number::one ^ number::two) == (enum_integer(number::one) ^ enum_integer(number::two)));
}

TEST(Flags, bitwise_operator5) {
  Color x1 = Color::RED;
  x1 |= Color::BLUE;
  EXPECT_TRUE(enum_integer(x1) == (enum_integer(Color::RED) | enum_integer(Color::BLUE)));

  Numbers x2 = Numbers::one;
  x2 |= Numbers::two;
  EXPECT_TRUE(enum_integer(x2) == (enum_integer(Numbers::one) | enum_integer(Numbers::two)));

  Directions x3 = Directions::Up;
  x3 |= Directions::Down;
  EXPECT_TRUE(enum_integer(x3) == (enum_integer(Directions::Up) | enum_integer(Directions::Down)));

  number x4 = number::one;
  x4 |= number::two;
  EXPECT_TRUE(enum_integer(x4) == (enum_integer(number::one) | enum_integer(number::two)));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  Language x5 = Language::日本語;
  x5 |= Language::한국어;
  EXPECT_TRUE(enum_integer(x5) == (enum_integer(Language::日本語) | enum_integer(Language::한국어)));
#endif
}

TEST(Flags, bitwise_operator6) {
  Color x1 = Color::RED;
  x1 &= Color::BLUE;
  EXPECT_TRUE(enum_integer(x1) == (enum_integer(Color::RED) & enum_integer(Color::BLUE)));

  Numbers x2 = Numbers::one;
  x2 &= Numbers::two;
  EXPECT_TRUE(enum_integer(x2) == (enum_integer(Numbers::one) & enum_integer(Numbers::two)));

  Directions x3 = Directions::Up;
  x3 &= Directions::Down;
  EXPECT_TRUE(enum_integer(x3) == (enum_integer(Directions::Up) & enum_integer(Directions::Down)));

  number x4 = number::one;
  x4 &= number::two;
  EXPECT_TRUE(enum_integer(x4) == (enum_integer(number::one) & enum_integer(number::two)));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  Language x5 = Language::日本語;
  x5 &= Language::한국어;
  EXPECT_TRUE(enum_integer(x5) == (enum_integer(Language::日本語) & enum_integer(Language::한국어)));
#endif
}

TEST(Flags, bitwise_operator7) {
  Color x1 = Color::RED;
  x1 ^= Color::BLUE;
  EXPECT_TRUE(enum_integer(x1) == (enum_integer(Color::RED) ^ enum_integer(Color::BLUE)));

  Numbers x2 = Numbers::one;
  x2 ^= Numbers::two;
  EXPECT_TRUE(enum_integer(x2) == (enum_integer(Numbers::one) ^ enum_integer(Numbers::two)));

  Directions x3 = Directions::Up;
  x3 ^= Directions::Down;
  EXPECT_TRUE(enum_integer(x3) == (enum_integer(Directions::Up) ^ enum_integer(Directions::Down)));

  number x4 = number::one;
  x4 ^= number::two;
  EXPECT_TRUE(enum_integer(x4) == (enum_integer(number::one) ^ enum_integer(number::two)));

#if defined(MAGIC_ENUM_ENABLE_NONASCII)
  Language x5 = Language::日本語;
  x5 ^= Language::한국어;
  EXPECT_TRUE(enum_integer(x5) == (enum_integer(Language::日本語) ^ enum_integer(Language::한국어)));
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

TEST(Flags, constexpr_for2) {
  constexpr_for<0, magic_enum::enum_count<Color>(), 1>([](auto i) {
    [[maybe_unused]] Foo<Color, magic_enum::enum_value<Color, i>()> bar{};
  });
}

#endif

#if defined(__cpp_lib_format)

#include <rendu_format.hpp>

TEST("format-base") {
  EXPECT_TRUE(std::format("Test-{:~^11}.", Color::RED | Color::GREEN) == "Test-~RED|GREEN~.");
}

#endif
