/*
* Created by boil on 2023/2/19.
*/

#include <test/rdtest.h>
#include <core/base/hashed_string.h>

template<typename>
struct foobar_t;

template<>
struct foobar_t<std::uint32_t> {
  static constexpr auto value = 0xbf9cf968;
};

template<>
struct foobar_t<std::uint64_t> {
  static constexpr auto value = 0x85944171f73967e8;
};

inline constexpr auto foobar_v = foobar_t<rendu::id_type>::value;

TEST(BasicHashedString, DeductionGuide) {
  static_assert(std::is_same_v<decltype(rendu::basic_hashed_string{"foo"}), rendu::hashed_string>);
  static_assert(std::is_same_v<decltype(rendu::basic_hashed_string{L"foo"}), rendu::hashed_wstring>);
}

TEST(HashedString, Functionalities) {
  using namespace rendu::literals;
  using hash_type = rendu::hashed_string::hash_type;

  const char *bar = "bar";

  auto foo_hs = rendu::hashed_string{"foo"};
  auto bar_hs = rendu::hashed_string{bar};

  ASSERT_NE(static_cast<hash_type>(foo_hs), static_cast<hash_type>(bar_hs));
  ASSERT_STREQ(static_cast<const char *>(foo_hs), "foo");
  ASSERT_STREQ(static_cast<const char *>(bar_hs), bar);
  ASSERT_STREQ(foo_hs.data(), "foo");
  ASSERT_STREQ(bar_hs.data(), bar);
  ASSERT_EQ(foo_hs.size(), 3u);
  ASSERT_EQ(bar_hs.size(), 3u);

  ASSERT_EQ(foo_hs, foo_hs);
  ASSERT_NE(foo_hs, bar_hs);

  rendu::hashed_string hs{"foobar"};

  ASSERT_EQ(static_cast<hash_type>(hs), foobar_v);
  ASSERT_EQ(hs.value(), foobar_v);

  ASSERT_EQ(foo_hs, "foo"_hs);
  ASSERT_NE(bar_hs, "foo"_hs);

  rendu::hashed_string empty_hs{};

  ASSERT_EQ(empty_hs, rendu::hashed_string{});
  ASSERT_NE(empty_hs, foo_hs);

  empty_hs = foo_hs;

  ASSERT_NE(empty_hs, rendu::hashed_string{});
  ASSERT_EQ(empty_hs, foo_hs);
}

TEST(HashedString, Empty) {
  using hash_type = rendu::hashed_string::hash_type;

  rendu::hashed_string hs{};

  ASSERT_EQ(hs.size(), 0u);
  ASSERT_EQ(static_cast<hash_type>(hs), hash_type{});
  ASSERT_EQ(static_cast<const char *>(hs), nullptr);
}

TEST(HashedString, Correctness) {
  const char *foobar = "foobar";
  std::string_view view{"foobar__", 6};

  ASSERT_EQ(rendu::hashed_string{foobar}, foobar_v);
  ASSERT_EQ((rendu::hashed_string{view.data(), view.size()}), foobar_v);
  ASSERT_EQ(rendu::hashed_string{"foobar"}, foobar_v);

  ASSERT_EQ(rendu::hashed_string::value(foobar), foobar_v);
  ASSERT_EQ(rendu::hashed_string::value(view.data(), view.size()), foobar_v);
  ASSERT_EQ(rendu::hashed_string::value("foobar"), foobar_v);

  ASSERT_EQ(rendu::hashed_string{foobar}.size(), 6u);
  ASSERT_EQ((rendu::hashed_string{view.data(), view.size()}).size(), 6u);
  ASSERT_EQ(rendu::hashed_string{"foobar"}.size(), 6u);
}

TEST(HashedString, Order) {
  using namespace rendu::literals;
  const rendu::hashed_string lhs = "foo"_hs;
  const rendu::hashed_string rhs = "bar"_hs;

  ASSERT_FALSE(lhs < lhs);
  ASSERT_FALSE(rhs < rhs);

  ASSERT_LT(rhs, lhs);
  ASSERT_LE(rhs, lhs);

  ASSERT_GT(lhs, rhs);
  ASSERT_GE(lhs, rhs);
}

TEST(HashedString, Constexprness) {
  using namespace rendu::literals;
  constexpr std::string_view view{"foobar__", 6};

  static_assert(rendu::hashed_string{"quux"} == "quux"_hs);
  static_assert(rendu::hashed_string{"foobar"} == foobar_v);

  static_assert(rendu::hashed_string::value("quux") == "quux"_hs);
  static_assert(rendu::hashed_string::value("foobar") == foobar_v);

  static_assert(rendu::hashed_string{"quux", 4} == "quux"_hs);
  static_assert(rendu::hashed_string{view.data(), view.size()} == foobar_v);

  static_assert(rendu::hashed_string::value("quux", 4) == "quux"_hs);
  static_assert(rendu::hashed_string::value(view.data(), view.size()) == foobar_v);

  static_assert(rendu::hashed_string{"bar"} < "foo"_hs);
  static_assert(rendu::hashed_string{"bar"} <= "bar"_hs);

  static_assert(rendu::hashed_string{"foo"} > "bar"_hs);
  static_assert(rendu::hashed_string{"foo"} >= "foo"_hs);
}

TEST(HashedWString, Functionalities) {
  using namespace rendu::literals;
  using hash_type = rendu::hashed_wstring::hash_type;

  const wchar_t *bar = L"bar";

  auto foo_hws = rendu::hashed_wstring{L"foo"};
  auto bar_hws = rendu::hashed_wstring{bar};

  ASSERT_NE(static_cast<hash_type>(foo_hws), static_cast<hash_type>(bar_hws));
  ASSERT_STREQ(static_cast<const wchar_t *>(foo_hws), L"foo");
  ASSERT_STREQ(static_cast<const wchar_t *>(bar_hws), bar);
  ASSERT_STREQ(foo_hws.data(), L"foo");
  ASSERT_STREQ(bar_hws.data(), bar);
  ASSERT_EQ(foo_hws.size(), 3u);
  ASSERT_EQ(bar_hws.size(), 3u);

  ASSERT_EQ(foo_hws, foo_hws);
  ASSERT_NE(foo_hws, bar_hws);

  rendu::hashed_wstring hws{L"foobar"};

  ASSERT_EQ(static_cast<hash_type>(hws), foobar_v);
  ASSERT_EQ(hws.value(), foobar_v);

  ASSERT_EQ(foo_hws, L"foo"_hws);
  ASSERT_NE(bar_hws, L"foo"_hws);
}

TEST(HashedWString, Empty) {
  using hash_type = rendu::hashed_wstring::hash_type;

  rendu::hashed_wstring hws{};

  ASSERT_EQ(hws.size(), 0u);
  ASSERT_EQ(static_cast<hash_type>(hws), hash_type{});
  ASSERT_EQ(static_cast<const wchar_t *>(hws), nullptr);
}

TEST(HashedWString, Correctness) {
  const wchar_t *foobar = L"foobar";
  std::wstring_view view{L"foobar__", 6};

  ASSERT_EQ(rendu::hashed_wstring{foobar}, foobar_v);
  ASSERT_EQ((rendu::hashed_wstring{view.data(), view.size()}), foobar_v);
  ASSERT_EQ(rendu::hashed_wstring{L"foobar"}, foobar_v);

  ASSERT_EQ(rendu::hashed_wstring::value(foobar), foobar_v);
  ASSERT_EQ(rendu::hashed_wstring::value(view.data(), view.size()), foobar_v);
  ASSERT_EQ(rendu::hashed_wstring::value(L"foobar"), foobar_v);

  ASSERT_EQ(rendu::hashed_wstring{foobar}.size(), 6u);
  ASSERT_EQ((rendu::hashed_wstring{view.data(), view.size()}).size(), 6u);
  ASSERT_EQ(rendu::hashed_wstring{L"foobar"}.size(), 6u);
}

TEST(HashedWString, Order) {
  using namespace rendu::literals;
  const rendu::hashed_wstring lhs = L"foo"_hws;
  const rendu::hashed_wstring rhs = L"bar"_hws;

  ASSERT_FALSE(lhs < lhs);
  ASSERT_FALSE(rhs < rhs);

  ASSERT_LT(rhs, lhs);
  ASSERT_LE(rhs, lhs);

  ASSERT_GT(lhs, rhs);
  ASSERT_GE(lhs, rhs);
}

TEST(HashedWString, Constexprness) {
  using namespace rendu::literals;
  constexpr std::wstring_view view{L"foobar__", 6};

  static_assert(rendu::hashed_wstring{L"quux"} == L"quux"_hws);
  static_assert(rendu::hashed_wstring{L"foobar"} == foobar_v);

  static_assert(rendu::hashed_wstring::value(L"quux") == L"quux"_hws);
  static_assert(rendu::hashed_wstring::value(L"foobar") == foobar_v);

  static_assert(rendu::hashed_wstring{L"quux", 4} == L"quux"_hws);
  static_assert(rendu::hashed_wstring{view.data(), view.size()} == foobar_v);

  static_assert(rendu::hashed_wstring::value(L"quux", 4) == L"quux"_hws);
  static_assert(rendu::hashed_wstring::value(view.data(), view.size()) == foobar_v);

  static_assert(rendu::hashed_wstring{L"bar"} < L"foo"_hws);
  static_assert(rendu::hashed_wstring{L"bar"} <= L"bar"_hws);

  static_assert(rendu::hashed_wstring{L"foo"} > L"bar"_hws);
  static_assert(rendu::hashed_wstring{L"foo"} >= L"foo"_hws);
}

