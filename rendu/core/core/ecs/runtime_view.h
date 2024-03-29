/*
* Created by boil on 2023/2/19.
*/

#ifndef RENDU_CORE_ECS_RUNTIME_VIEW_H_
#define RENDU_CORE_ECS_RUNTIME_VIEW_H_

#include <vector>
#include "base/iterator.h"
#include "entity.h"
#include "fwd.h"

namespace rendu {

namespace internal {

template<typename Set>
class runtime_view_iterator final {
  using iterator_type = typename Set::iterator;

  [[nodiscard]] bool valid() const {
    return (!tombstone_check || *it != tombstone)
        && std::all_of(++pools->begin(), pools->end(), [entity = *it](const auto *curr) { return curr->contains(entity); })
        && std::none_of(filter->cbegin(), filter->cend(), [entity = *it](const auto *curr) { return curr && curr->contains(entity); });
  }

 public:
  using difference_type = typename iterator_type::difference_type;
  using value_type = typename iterator_type::value_type;
  using pointer = typename iterator_type::pointer;
  using reference = typename iterator_type::reference;
  using iterator_category = std::bidirectional_iterator_tag;

  constexpr runtime_view_iterator() noexcept
      : pools{},
        filter{},
        it{},
        tombstone_check{} {}

  runtime_view_iterator(const std::vector<Set *> &cpools, const std::vector<Set *> &ignore, iterator_type curr) noexcept
      : pools{&cpools},
        filter{&ignore},
        it{curr},
        tombstone_check{pools->size() == 1u && (*pools)[0u]->policy() == deletion_policy::in_place} {
    if(it != (*pools)[0]->end() && !valid()) {
      ++(*this);
    }
  }

  runtime_view_iterator &operator++() {
    while(++it != (*pools)[0]->end() && !valid()) {}
    return *this;
  }

  runtime_view_iterator operator++(int) {
    runtime_view_iterator orig = *this;
    return ++(*this), orig;
  }

  runtime_view_iterator &operator--() {
    while(--it != (*pools)[0]->begin() && !valid()) {}
    return *this;
  }

  runtime_view_iterator operator--(int) {
    runtime_view_iterator orig = *this;
    return operator--(), orig;
  }

  [[nodiscard]] pointer operator->() const noexcept {
    return it.operator->();
  }

  [[nodiscard]] reference operator*() const noexcept {
    return *operator->();
  }

  [[nodiscard]] constexpr bool operator==(const runtime_view_iterator &other) const noexcept {
    return it == other.it;
  }

  [[nodiscard]] constexpr bool operator!=(const runtime_view_iterator &other) const noexcept {
    return !(*this == other);
  }

 private:
  const std::vector<Set *> *pools;
  const std::vector<Set *> *filter;
  iterator_type it;
  bool tombstone_check;
};

} // namespace internal

/**
 * @brief Generic runtime view.
 *
 * Runtime views iterate over those entities that have at least all the given
 * components in their bags. During initialization, a runtime view looks at the
 * number of entities available for each component and picks up a reference to
 * the smallest set of candidate entities in order to get a performance boost
 * when iterate.<br/>
 * Order of elements during iterations are highly dependent on the order of the
 * underlying data structures. See sparse_set and its specializations for more
 * details.
 *
 * @b Important
 *
 * Iterators aren't invalidated if:
 *
 * * New instances of the given components are created and assigned to entities.
 * * The entity currently pointed is modified (as an example, if one of the
 *   given components is removed from the entity to which the iterator points).
 * * The entity currently pointed is destroyed.
 *
 * In all the other cases, modifying the pools of the given components in any
 * way invalidates all the iterators and using them results in undefined
 * behavior.
 *
 * @note
 * Views share references to the underlying data structures of the registry that
 * generated them. Therefore any change to the entities and to the components
 * made by means of the registry are immediately reflected by the views, unless
 * a pool was missing when the view was built (in this case, the view won't
 * have a valid reference and won't be updated accordingly).
 *
 * @warning
 * Lifetime of a view must not overcome that of the registry that generated it.
 * In any other case, attempting to use a view results in undefined behavior.
 *
 * @tparam Type Common base type.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Type, typename Allocator>
class basic_runtime_view {
  using alloc_traits = std::allocator_traits<Allocator>;
  static_assert(std::is_same_v<typename alloc_traits::value_type, Type *>, "Invalid value type");
  using container_type = std::vector<Type *, Allocator>;

 public:
  /*! @brief Allocator type. */
  using allocator_type = Allocator;
  /*! @brief Underlying entity identifier. */
  using entity_type = typename Type::entity_type;
  /*! @brief Unsigned integer type. */
  using size_type = std::size_t;
  /*! @brief Common type among all storage types. */
  using base_type = Type;
  /*! @brief Bidirectional iterator type. */
  using iterator = internal::runtime_view_iterator<base_type>;

  /*! @brief Default constructor to use to create empty, invalid views. */
  basic_runtime_view() noexcept
      : basic_runtime_view{allocator_type{}} {}

  /**
   * @brief Constructs an empty, invalid view with a given allocator.
   * @param allocator The allocator to use.
   */
  explicit basic_runtime_view(const allocator_type &allocator)
      : pools{allocator},
        filter{allocator} {}

  /*! @brief Default copy constructor. */
  basic_runtime_view(const basic_runtime_view &) = default;

  /**
   * @brief Allocator-extended copy constructor.
   * @param other The instance to copy from.
   * @param allocator The allocator to use.
   */
  basic_runtime_view(const basic_runtime_view &other, const allocator_type &allocator)
      : pools{other.pools, allocator},
        filter{other.filter, allocator} {}

  /*! @brief Default move constructor. */
  basic_runtime_view(basic_runtime_view &&) noexcept(std::is_nothrow_move_constructible_v<container_type>) = default;

  /**
   * @brief Allocator-extended move constructor.
   * @param other The instance to move from.
   * @param allocator The allocator to use.
   */
  basic_runtime_view(basic_runtime_view &&other, const allocator_type &allocator)
      : pools{std::move(other.pools), allocator},
        filter{std::move(other.filter), allocator} {}

  /**
   * @brief Default copy assignment operator.
   * @return This container.
   */
  basic_runtime_view &operator=(const basic_runtime_view &) = default;

  /**
   * @brief Default move assignment operator.
   * @return This container.
   */
  basic_runtime_view &operator=(basic_runtime_view &&) noexcept(std::is_nothrow_move_assignable_v<container_type>) = default;

  /**
   * @brief Exchanges the contents with those of a given view.
   * @param other View to exchange the content with.
   */
  void swap(basic_runtime_view &other) {
    using std::swap;
    swap(pools, other.pools);
    swap(filter, other.filter);
  }

  /**
   * @brief Returns the associated allocator.
   * @return The associated allocator.
   */
  [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
    return pools.get_allocator();
  }

  /*! @brief Clears the view. */
  void clear() {
    pools.clear();
    filter.clear();
  }

  /**
   * @brief Appends an opaque storage object to a runtime view.
   * @param base An opaque reference to a storage object.
   * @return This runtime view.
   */
  basic_runtime_view &iterate(base_type &base) {
    if(pools.empty() || !(base.size() < pools[0u]->size())) {
      pools.push_back(&base);
    } else {
      pools.push_back(std::exchange(pools[0u], &base));
    }

    return *this;
  }

  /**
   * @brief Adds an opaque storage object as a filter of a runtime view.
   * @param base An opaque reference to a storage object.
   * @return This runtime view.
   */
  basic_runtime_view &exclude(base_type &base) {
    filter.push_back(&base);
    return *this;
  }

  /**
   * @brief Estimates the number of entities iterated by the view.
   * @return Estimated number of entities iterated by the view.
   */
  [[nodiscard]] size_type size_hint() const {
    return pools.empty() ? size_type{} : pools.front()->size();
  }

  /**
   * @brief Returns an iterator to the first entity that has the given
   * components.
   *
   * The returned iterator points to the first entity that has the given
   * components. If the view is empty, the returned iterator will be equal to
   * `end()`.
   *
   * @return An iterator to the first entity that has the given components.
   */
  [[nodiscard]] iterator begin() const {
    return pools.empty() ? iterator{} : iterator{pools, filter, pools[0]->begin()};
  }

  /**
   * @brief Returns an iterator that is past the last entity that has the
   * given components.
   *
   * The returned iterator points to the entity following the last entity that
   * has the given components. Attempting to dereference the returned iterator
   * results in undefined behavior.
   *
   * @return An iterator to the entity following the last entity that has the
   * given components.
   */
  [[nodiscard]] iterator end() const {
    return pools.empty() ? iterator{} : iterator{pools, filter, pools[0]->end()};
  }

  /**
   * @brief Checks if a view contains an entity.
   * @param entity A valid identifier.
   * @return True if the view contains the given entity, false otherwise.
   */
  [[nodiscard]] bool contains(const entity_type entity) const {
    return !pools.empty()
        && std::all_of(pools.cbegin(), pools.cend(), [entity](const auto *curr) { return curr->contains(entity); })
        && std::none_of(filter.cbegin(), filter.cend(), [entity](const auto *curr) { return curr && curr->contains(entity); });
  }

  /**
   * @brief Iterates entities and applies the given function object to them.
   *
   * The function object is invoked for each entity. It is provided only with
   * the entity itself. To get the components, users can use the registry with
   * which the view was built.<br/>
   * The signature of the function should be equivalent to the following:
   *
   * @code{.cpp}
   * void(const entity_type);
   * @endcode
   *
   * @tparam Func Type of the function object to invoke.
   * @param func A valid function object.
   */
  template<typename Func>
  void each(Func func) const {
    for(const auto entity: *this) {
      func(entity);
    }
  }

 private:
  container_type pools;
  container_type filter;
};

} // namespace rendu
#endif //RENDU_CORE_ECS_RUNTIME_VIEW_H_
