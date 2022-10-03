/*
* Created by boil on 2022/9/17.
*/

#ifndef RENDU_ARGUMENT_DEFINES_H_
#define RENDU_ARGUMENT_DEFINES_H_

#include "argument_template.h"
#include "enum.h"

namespace rendu {
  enum class default_arguments : unsigned int {
    none = 0,
    help = 1,
    version = 2,
    all = help | version,
  };

  enum class nargs_pattern {
    optional,
    any,
    at_least_one
  };

  inline default_arguments operator&(const default_arguments &a,
                                     const default_arguments &b) {
    return static_cast<default_arguments>(
        static_cast<std::underlying_type<default_arguments>::type>(a) &
        static_cast<std::underlying_type<default_arguments>::type>(b));
  }

}//namespace rendu
#endif //RENDU_ARGUMENT_DEFINES_H_
