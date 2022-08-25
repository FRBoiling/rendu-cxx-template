/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#ifndef RenduCore_Optional_h__
#define RenduCore_Optional_h__

#include <optional>

//! Optional helper class to wrap optional values within.
template <class T>
using Optional = std::optional<T>;

#endif // RenduCore_Optional_h__
