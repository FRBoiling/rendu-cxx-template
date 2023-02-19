/*
* Created by boil on 2022/10/2.
*/
#include <test/rdtest.h>
#include <set>

RD_TEST(container,set) {
  std::set<int> set1;
  set1.insert(1);
  set1.insert(2);
  set1.insert(1);

  RD_EXPECT_TRUE(set1.size() == 2);
  set1.erase(2);

  RD_EXPECT_TRUE(set1.size() == 1);
}