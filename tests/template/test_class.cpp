//
// Created by boil on 2023/2/14.
//
#include <test/rdtest.h>

template<typename T>
class A {
 public:
  A(T y) : x(y) {}

 private:
  T x;
 public:
  T get_value() {
    return x;
  }

};

TEST(TemplateTest, Class) {
  A<int> a(10);
  EXPECT_EQ(10, a.get_value());
  A<float> b(0.1f);
  EXPECT_EQ(0.1f, b.get_value());
}

// 变量模板
template<typename T>
T zero = 0;

// 别名模板
template<typename T>
using Container = std::vector<T>;

TEST(TemplateTest, Alias) {
  auto i = zero<int> = 11; // 相当于int i = 11;
  Container<int> v{1, 2, 3}; // 相当于std::vector<int> v{1, 2, 3};
  EXPECT_EQ(11, i);
  EXPECT_EQ(2, v[1]);
}
