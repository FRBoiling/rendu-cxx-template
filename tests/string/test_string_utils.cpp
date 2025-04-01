/*
* Created by boil on 2023/2/14.
*/
#include <test/rdtest.h>
#include <common/utils/string_util.h>

TEST(StringTest, TruncSubstr) {
  const std::string_view hi("hi");
  EXPECT_EQ("", rendu::ClippedSubstr(hi, 0, 0));
  EXPECT_EQ("h", rendu::ClippedSubstr(hi, 0, 1));
  EXPECT_EQ("hi", rendu::ClippedSubstr(hi, 0));
  EXPECT_EQ("i", rendu::ClippedSubstr(hi, 1));
  EXPECT_EQ("", rendu::ClippedSubstr(hi, 2));
  EXPECT_EQ("", rendu::ClippedSubstr(hi, 3));  // truncation
  EXPECT_EQ("", rendu::ClippedSubstr(hi, 3, 2));  // truncation
}

TEST(StringTest, NullSafeStringView) {
  {
    std::string_view s = rendu::NullSafeStringView(nullptr);
    EXPECT_EQ(nullptr, s.data());
    EXPECT_EQ(0u, s.size());
    EXPECT_EQ(std::string_view(), s);
  }
  {
    static const char kHi[] = "hi";
    std::string_view s = rendu::NullSafeStringView(kHi);
    EXPECT_EQ(kHi, s.data());
    EXPECT_EQ(strlen(kHi), s.size());
    EXPECT_EQ(std::string_view("hi"), s);
  }
}

TEST(StringTest, ConstexprNullSafeStringView) {
  {
    constexpr std::string_view s = rendu::NullSafeStringView(nullptr);
    EXPECT_EQ(nullptr, s.data());
    EXPECT_EQ(0u, s.size());
    EXPECT_EQ(std::string_view(), s);
  }
#if !defined(_MSC_VER) || _MSC_VER >= 1910
// MSVC 2017+ is required for good constexpr string_view support.
// See the implementation of `std::string_view::StrlenInternal()`.
  {
    static constexpr char kHi[] = "hi";
    std::string_view s = rendu::NullSafeStringView(kHi);
    EXPECT_EQ(kHi, s.data());
    EXPECT_EQ(strlen(kHi), s.size());
    EXPECT_EQ(std::string_view("hi"), s);
  }
  {
    constexpr std::string_view s = rendu::NullSafeStringView("hello");
    EXPECT_EQ(s.size(), 5u);
    EXPECT_EQ("hello", s);
  }
#endif
}
