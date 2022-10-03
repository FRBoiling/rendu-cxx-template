/*
* Created by boil on 2022/9/25.
*/

#ifndef RENDU_ENUM_UTILS_H_
#define RENDU_ENUM_UTILS_H_

#include "enum.h"

namespace rendu {
  class EnumUtils {
  public:

    template<typename Enum>
    static std::string ToString(Enum value) {
      std::string enum_str;
      enum_str.append(enum_type_name<Enum>());
      enum_str.append("::");
      enum_str.append(enum_flags_name(value));
      return enum_str;
    }
  };

}//namespace rendu

#endif
