/*
* Created by boil on 2023/02/19.
*/

#include <magic_enum/magic_enum.hpp>
#include <iostream>

using namespace std;

enum class EnumClass{
  Num0,
  Num1,
  Num2
};

enum EnumNormal{
  Num0,
  Num1,
  Num2
};

int main(int argc, char **argv) {


  int a = magic_enum::enum_integer(EnumClass::Num1);
  int b = magic_enum::enum_integer(EnumNormal::Num1);

  auto c = magic_enum::enum_cast<EnumNormal>(1);

  auto d =  c.value();
  auto e = c.has_value();

  auto f1 = magic_enum::enum_integer(EnumClass::Num1);
  auto f2 = magic_enum::enum_index(EnumClass::Num1);

  auto g = magic_enum::enum_contains<EnumClass>(EnumClass::Num1);

  auto h = magic_enum::enum_flags_name(EnumClass::Num1);

  auto i = magic_enum::enum_name(EnumClass::Num1);

  std::cout << a << std::endl;

  return 0;
}