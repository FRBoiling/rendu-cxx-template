/*
* Created by boil on 2022/10/2.
*/
#include "rd_catch.h"
#include <set>

TEST_CASE("tests_set") {
  std::set<int> set1;
  set1.insert(1);
  set1.insert(2);
  set1.insert(1);

  REQUIRE(set1.size() == 2);
  set1.erase(2);

  REQUIRE(set1.size() == 1);


}