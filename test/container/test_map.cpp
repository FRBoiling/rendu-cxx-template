/*
* Created by boil on 2022/10/2.
*/
#include <test/rdtest.h>
#include <map>

RD_TEST(container,map) {

  std::map<int,int> map1;
  map1[1] =11;
  map1[2] =22;
  map1[2] =33;

  RD_EXPECT_TRUE(map1.size() == 2);
  map1.erase(1);

  RD_EXPECT_TRUE(map1.size() == 2);

}