/*
* Created by boil on 2022/9/24.
*/

#pragma once
#include <iostream>
#include "enum/enum_utils.h"
#include "log.h"
using namespace rendu;
using namespace rendu::iostream_operators;

enum NormalEnum{
  One = 1,
  Two = 2,
  Three = 3,
};

enum class ClassEnum{
  One = 11,
  Two = 22,
  Three = 33,
};


void example_enum(){
  std::cout<< One << std::endl;
  std::cout<< enum_name(ClassEnum::One) << std::endl;

  std::cout << EnumUtils::ToString(Two) << std::endl;
  std::cout << EnumUtils::ToString(ClassEnum::Two) << std::endl;

  int tt = Three + NormalEnum::Three;

  std::cout<< tt << std::endl;

  auto one = enum_value<ClassEnum>(0);
  auto two = enum_cast<ClassEnum>(44);
  if (!two){
    two = enum_cast<ClassEnum>(33).value();
    std::cout << two << std::endl;
  }

  auto index = enum_index(ClassEnum::One).value();

  enum_for_each<ClassEnum>([](auto val) {
    constexpr underlying_type_t<NormalEnum> v = enum_integer(val());
    std::cout << v<< std::endl;
  });

  std::cout << enum_name(one) << std::endl;
  std::cout << enum_flags_name(one) << std::endl;

  int n_one = (int)ClassEnum::One;
  auto e_one = (ClassEnum)11;

  int n_n_one = One;
  auto e_n_one = (NormalEnum)1;

//  auto one = EnumUtils::FromIndex<NormalEnum>(Three);


}
