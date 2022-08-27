/*
* Created by boil on 2022/8/20.
*/

#ifndef RENDU_REGEX_H_
#define RENDU_REGEX_H_

#ifndef RD_HAS_BROKEN_WSTRING_REGEX
#include <regex>
#define RD_REGEX_NAMESPACE std
#else
#include <boost/regex.hpp>
#define RD_REGEX_NAMESPACE boost
#endif

// regex compatibility layer, required for clang building with libstdc++-4.9
namespace rendu
{
    using regex = RD_REGEX_NAMESPACE :: regex;
    using wregex = RD_REGEX_NAMESPACE :: wregex;

    using :: RD_REGEX_NAMESPACE :: regex_match;
    using :: RD_REGEX_NAMESPACE :: regex_search;
}

#endif // RENDU_REGEX_H_
