/*
* Created by boil on 2022/8/20.
*/

#ifndef RENDU_COMMON_H_
#define RENDU_COMMON_H_

#include "define.h"
#include <array>
#include <memory>
#include <string>
#include <utility>

#if RENDU_PLATFORM == RENDU_PLATFORM_WINDOWS
#  if RENDU_COMPILER == RENDU_COMPILER_INTEL
#    if !defined(BOOST_ASIO_HAS_MOVE)
#      define BOOST_ASIO_HAS_MOVE
#    endif // !defined(BOOST_ASIO_HAS_MOVE)
#  endif // if RENDU_COMPILER == RENDU_COMPILER_INTEL
#else

#  include <sys/types.h>
#  include <sys/ioctl.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <unistd.h>
#  include <netdb.h>
#  include <cstdlib>

#endif

#if RENDU_COMPILER == RENDU_COMPILER_MICROSOFT

#define atoll _atoi64
#define llabs _abs64

#else

#define stricmp strcasecmp
#define strnicmp strncasecmp

#endif

inline unsigned long atoul(char const *str) { return strtoul(str, nullptr, 10); }

inline unsigned long long atoull(char const *str) { return strtoull(str, nullptr, 10); }

#define STRINGIZE(a) #a

enum TimeConstants {
  MINUTE = 60,
  HOUR = MINUTE * 60,
  DAY = HOUR * 24,
  WEEK = DAY * 7,
  MONTH = DAY * 30,
  YEAR = MONTH * 12,
  IN_MILLISECONDS = 1000
};

enum AccountTypes {
  SEC_PLAYER = 0,
  SEC_MODERATOR = 1,
  SEC_GAMEMASTER = 2,
  SEC_ADMINISTRATOR = 3,
  SEC_CONSOLE = 4                                  // must be always last in list, accounts must have less security level always also
};

enum LocaleConstant : uint8 {
  LOCALE_enUS = 0,
  LOCALE_koKR = 1,
  LOCALE_frFR = 2,
  LOCALE_deDE = 3,
  LOCALE_zhCN = 4,
  LOCALE_zhTW = 5,
  LOCALE_esES = 6,
  LOCALE_esMX = 7,
  LOCALE_ruRU = 8,
  LOCALE_none = 9,
  LOCALE_ptBR = 10,
  LOCALE_itIT = 11,

  TOTAL_LOCALES
};

const uint8 OLD_TOTAL_LOCALES = 9; /// @todo convert in simple system
#define DEFAULT_LOCALE LOCALE_enUS

enum class CascLocaleBit : uint8 {
  None = 0,
  enUS = 1,
  koKR = 2,
  Reserved = 3,
  frFR = 4,
  deDE = 5,
  zhCN = 6,
  esES = 7,
  zhTW = 8,
  enGB = 9,
  enCN = 10,
  enTW = 11,
  esMX = 12,
  ruRU = 13,
  ptBR = 14,
  itIT = 15,
  ptPT = 16
};

RD_COMMON_API extern char const *localeNames[TOTAL_LOCALES];

RD_COMMON_API LocaleConstant GetLocaleByName(std::string const &name);

RD_COMMON_API extern CascLocaleBit WowLocaleToCascLocaleBit[TOTAL_LOCALES];

constexpr inline bool IsValidLocale(LocaleConstant locale) {
  return locale < TOTAL_LOCALES && locale != LOCALE_none;
}

#pragma pack(push, 1)

struct LocalizedString {
  constexpr char const *operator[](LocaleConstant locale) const {
    return Str[locale];
  }

  std::array<char const *, TOTAL_LOCALES> Str;
};

#pragma pack(pop)

// we always use stdlib std::max/std::min, undefine some not C++ standard defines (Win API and some other platforms)
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_4
#define M_PI_4 0.785398163397448309616
#endif

#define MAX_QUERY_LEN 32*1024

#endif //RENDU_COMMON_H_
