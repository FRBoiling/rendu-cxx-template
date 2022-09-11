/*
* Created by boil on 2022/8/20.
*/

#ifndef RENDU_STRING_FORMAT_H_
#define RENDU_STRING_FORMAT_H_

#include "fmt/printf.h"

namespace rendu {
  /// Default RD string format function.
  template<typename Format, typename... Args>
  inline std::string StringFormat(Format &&fmt, Args &&... args) {
    try {
      return fmt::format(std::forward<Format>(fmt), std::forward<Args>(args)...);
    }
    catch (const fmt::format_error &formatError) {
      std::string error =
          "An error occurred formatting string \"" + std::string(fmt) + "\" : " + std::string(formatError.what());
      return error;
    }
  }

  /// Returns true if the given char pointer is null.
  inline bool IsFormatEmptyOrNull(char const *fmt) {
    return fmt == nullptr;
  }

  /// Returns true if the given std::string is empty.
  inline bool IsFormatEmptyOrNull(std::string const &fmt) {
    return fmt.empty();
  }
}

#endif //RENDU_STRING_FORMAT_H_
