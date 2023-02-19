/*
* Created by boil on 2022/10/2.
*/
#include <test/rdtest.h>
#include <vector>

namespace {

RD_TEST(container,vector) {
  std::vector<int> vec1;
  vec1.push_back(1);
  vec1.push_back(2);
  vec1.push_back(1);

  RD_EXPECT_TRUE(vec1.size() == 3);
  RD_EXPECT_TRUE(vec1.size() == 2);

}
}