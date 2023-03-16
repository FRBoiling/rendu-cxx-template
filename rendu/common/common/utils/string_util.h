/*
* Created by boil on 2023/2/14.
*/

#ifndef RENDU_COMMON_UTILS_STRING_UTIL_H_
#define RENDU_COMMON_UTILS_STRING_UTIL_H_

#include <string_view>

namespace rendu {

  inline std::string_view ClippedSubstr(std::string_view s, size_t pos,
                                        size_t n = std::string_view::npos) {
    pos = (std::min)(pos, static_cast<size_t>(s.size()));
    return s.substr(pos, n);
  }

  constexpr std::string_view NullSafeStringView(const char *p) {
    return p ? std::string_view(p) : std::string_view();
  }

}//namespace rendu

#endif //RENDU_COMMON_UTILS_STRING_UTIL_H_
