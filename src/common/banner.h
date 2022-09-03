/*
* Created by boil on 2022/8/20.
*/
#ifndef RENDU_BANNER_H_
#define RENDU_BANNER_H_

#include "define.h"

namespace rendu::banner {
    RD_COMMON_API void Show(char const *applicationName, void(*log)(char const *text), void(*logExtraInfo)());
  }

#endif // RENDU_BANNER_H_
