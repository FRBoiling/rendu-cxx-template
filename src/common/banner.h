/**********************************
* Created by boil on 2022/8/14.
***********************************/

#ifndef RenduCore_Banner_h__
#define RenduCore_Banner_h__

#include "define.h"

namespace rendu {
  namespace Banner {
    RD_COMMON_API void Show(char const *applicationName, void(*log)(char const *text), void(*logExtraInfo)());
  }
}

#endif // RenduCore_Banner_h__
