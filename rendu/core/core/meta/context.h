/*
* Created by boil on 2023/2/22.
*/

#ifndef RENDU_CORE_META_CONTEXT_H_
#define RENDU_CORE_META_CONTEXT_H_

#include "base/utility.h"
#include "container/dense_map.h"

namespace rendu {

class meta_ctx;

namespace internal {

struct meta_type_node;

struct meta_context {
  dense_map<id_type, meta_type_node, identity> value{};

  [[nodiscard]] static inline meta_context &from(meta_ctx &ctx);
  [[nodiscard]] static inline const meta_context &from(const meta_ctx &ctx);
};

} // namespace internal

/*! @brief Disambiguation tag for constructors and the like. */
class meta_ctx_arg_t final {};

/*! @brief Constant of type meta_context_arg_t used to disambiguate calls. */
inline constexpr meta_ctx_arg_t meta_ctx_arg{};

/*! @brief Opaque meta context type. */
class meta_ctx : private internal::meta_context {
  /*! @brief Attorney idiom like model to access the base class. */
  friend struct internal::meta_context;
};

/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */

[[nodiscard]] inline internal::meta_context &internal::meta_context::from(meta_ctx &ctx) {
  return ctx;
}

[[nodiscard]] inline const internal::meta_context &internal::meta_context::from(const meta_ctx &ctx) {
  return ctx;
}

} // namespace rendu

#endif //RENDU_CORE_META_CONTEXT_H_
