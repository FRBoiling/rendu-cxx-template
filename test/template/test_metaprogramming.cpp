//
// Created by boil on 2023/2/14.
//
#include <test/rdtest.h>


template<int N>
class Fac {
 public:
  enum {
    value = N * Fac<N - 1>::value
  };
};

template<>
class Fac<0> {
 public:
  enum {
    value = 1
  };
};

template<int N>
class Fac1 {
 public:
  static const int value = N * Fac1<N - 1>::value;
};

template<>
class Fac1<0> {
 public:
  static const int value = 1;
};

template<int N>
class Fac2 {
 public:
  static constexpr auto value = N * Fac2<N - 1>::value;
};

template<>
class Fac2<0> {
 public:
  static constexpr auto value = 1;
};

template<int N>
class Fac3 {
 public:
  static inline constexpr auto value = N * Fac3<N - 1>::value;
};

template<>
class Fac3<0> {
 public:
  static inline constexpr auto value = 1;
};

int f(const int &a) {
  return a;
}  //函数参数是引用

//一个模板元编程一般包括：递归构造的手段、表示状态的模板参数、一个表示终点的特化以及具体实现的算法。
TEST(TemplateTest, Metaprogramming) {
  EXPECT_EQ(120, Fac<5>::value);
//  EXPECT_EQ(120, Fac1<5>::value); //TODO:BOIL 这个编译不通过。不知道什么原因
  EXPECT_EQ(120, Fac2<5>::value);
  EXPECT_EQ(120, f(Fac2<5>::value));
  EXPECT_EQ(120, f(Fac3<5>::value));
}
