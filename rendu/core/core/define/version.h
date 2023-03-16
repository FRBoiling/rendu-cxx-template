/*
* Created by boil on 2023/2/25.
*/

#ifndef RENDU_CORE_DEFINE_VERSION_H_
#define RENDU_CORE_DEFINE_VERSION_H_

#include "macro.h"

#define RD_VERSION_MAJOR 3
#define RD_VERSION_MINOR 12
#define RD_VERSION_PATCH 0

#define RD_VERSION \
    RD_XSTR(RD_VERSION_MAJOR) \
    "." RD_XSTR(RD_VERSION_MINOR) "." RD_XSTR(RD_VERSION_PATCH)

#endif //RENDU_CORE_DEFINE_VERSION_H_
