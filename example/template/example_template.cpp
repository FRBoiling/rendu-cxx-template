//
// Created by boil on 2023/2/14.
//
#include <iostream>

template<typename T>
T max(T x, T y) {
  return x < y ? y : x;
}

template<> // template<>表示这是一个特化版本
int64_t max<int64_t>(int64_t x, int64_t y) // 加上一个尖括号并指定特化类型，如果可以推断出也可以省略
{
  return x < y ? x : y; // 返回较小值
}


int main() {
  std::cout << ::max(10, 20) << std::endl;
  std::cout << ::max<int>(10, 20) << std::endl;
  std::cout << ::max(1.2, 2.2) << std::endl;
  std::cout << ::max<double>(1.2, 2.2) << std::endl;
  std::cout << ::max<int64_t>(10, 20) << std::endl;

  std::cout << "Hello Template" << std::endl;
}