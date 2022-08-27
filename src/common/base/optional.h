/**********************************
**  Created by boil on 2022/8/14.
***********************************/

#ifndef RENDU_OPTIONAL_H_
#define RENDU_OPTIONAL_H_

#include <optional>

//! Optional helper class to wrap optional values within.
template <class T>
using Optional = std::optional<T>;

#endif // RENDU_OPTIONAL_H_
