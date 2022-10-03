/*
* Created by boil on 2022/10/2.
*/
#include "rd_catch.h"
#include <vector>

TEST_CASE("tests_vector") {
  std::vector<int> vec1;
  vec1.push_back(1);
  vec1.push_back(2);
  vec1.push_back(1);

  REQUIRE(vec1.size() == 3);
  REQUIRE(vec1.size() == 2);


}