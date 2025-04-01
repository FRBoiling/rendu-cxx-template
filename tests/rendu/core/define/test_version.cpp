/*
* Created by boil on 2023/2/25.
*/

#include <test/rdtest.h>
#include <regex>
#include <core/define/version.h>

TEST(Version, All) {
  ASSERT_STREQ(RD_VERSION, RD_XSTR(RD_VERSION_MAJOR) "." RD_XSTR(RD_VERSION_MINOR) "." RD_XSTR
  (RD_VERSION_PATCH));
  ASSERT_TRUE(std::regex_match(RD_VERSION, std::regex{"^[0-9]+\\.[0-9]+\\.[0-9]+$"}));
}
