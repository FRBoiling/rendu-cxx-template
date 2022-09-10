/*
* Created by boil on 2022/9/10.
*/

#include <magic_enum.hpp>

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

int main(int, char *[]) {


  int a = magic_enum::enum_integer(EnumClass::Num1);
  int b = magic_enum::enum_integer(EnumNormal::Num1);

  auto c = magic_enum::enum_cast<EnumNormal>("");

  auto d =  c.value();
  auto e = c.has_value();
  return 0;
}