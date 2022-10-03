/*
* Created by boil on 2022/10/2.
*/
#include "rd_catch.h"
#include <map>

TEST_CASE("tests_map") {

  std::map<int,int> map1;
  map1[1] =11;
  map1[2] =22;
  map1[2] =33;

  REQUIRE(map1.size() == 2);
  map1.erase(1);

  REQUIRE(map1.size() == 2);


}