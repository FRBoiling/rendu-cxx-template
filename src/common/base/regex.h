/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#ifndef RenduCore_Regex_h__
#define RenduCore_Regex_h__

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

#endif // RenduCore_Regex_h__
