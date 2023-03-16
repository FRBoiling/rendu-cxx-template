/*
* Created by boil on 2023/2/16.
*/

#ifndef RENDU_CORE_BASE_FWD_H_
#define RENDU_CORE_BASE_FWD_H_

#include <cstddef>
#include "define/define.h"

namespace rendu {

template<std::size_t Len = sizeof(double[2]), std::size_t = alignof(double[2])>
class basic_any;

/*! @brief Alias declaration for type identifiers. */
using id_type = RD_ID_TYPE;

/*! @brief Alias declaration for the most common use case. */
using any = basic_any<>;

} // namespace rendu


#endif //RENDU_CORE_BASE_FWD_H_
