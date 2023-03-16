/*
* Created by boil on 2022/10/2.
*/
#include <test/rdtest.h>
#include <set>

TEST(container,set) {
  std::set<int> set1;
  set1.insert(1);
  set1.insert(2);
  set1.insert(1);

  EXPECT_TRUE(set1.size() == 2);
  set1.erase(2);

  EXPECT_TRUE(set1.size() == 1);
}