/*
* Created by boil on 2023/2/17.
*/

#ifndef RENDU_CORE_ECS_REGISTRY_H_
#define RENDU_CORE_ECS_REGISTRY_H_

#include "define/define.h"
#include "base/any.h"
#include "base/algorithm.h"
#include "container/dense_map.h"
#include "entity.h"
#include "storage.h"
#include "group.h"
#include "mixin.h"
#include "view.h"

namespace rendu {

namespace internal {

template<typename It>
class registry_storage_iterator final {
  template<typename Other>
  friend
  class registry_storage_iterator;
  using mapped_type = std::remove_reference_t<decltype(std::declval<It>()->second)>;

 public:
  using value_type = std::pair<id_type, constness_as_t<typename mapped_type::element_type, mapped_type> &>;
  using pointer = input_iterator_pointer<value_type>;
  using reference = value_type;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::input_iterator_tag;

  constexpr registry_storage_iterator() noexcept
      : it{} {}

  constexpr registry_storage_iterator(It iter) noexcept
      : it{iter} {}

  template<typename Other, typename =
  std::enable_if_t<!std::is_same_v<It, Other> && std::is_constructible_v<It, Other>>
  >
  constexpr registry_storage_iterator(const registry_storage_iterator<Other> &other) noexcept
      : registry_storage_iterator{other.it} {}

  constexpr registry_storage_iterator &operator++() noexcept {
    return ++it, *this;
  }

  constexpr registry_storage_iterator operator++(int) noexcept {
    registry_storage_iterator orig = *this;
    return ++(*this), orig;
  }

  constexpr registry_storage_iterator &operator--() noexcept {
    return --it, *this;
  }

  constexpr registry_storage_iterator operator--(int) noexcept {
    registry_storage_iterator orig = *this;
    return operator--(), orig;
  }

  constexpr registry_storage_iterator &operator+=(const difference_type value) noexcept {
    it += value;
    return *this;
  }

  constexpr registry_storage_iterator operator+(const difference_type value) const noexcept {
    registry_storage_iterator copy = *this;
    return (copy += value);
  }

  constexpr registry_storage_iterator &operator-=(const difference_type value) noexcept {
    return (*this += -value);
  }

  constexpr registry_storage_iterator operator-(const difference_type value) const noexcept {
    return (*this + -value);
  }

  [[nodiscard]] constexpr reference operator[](const difference_type value) const noexcept {
    return {it[value].first, *it[value].second};
  }

  [[nodiscard]] constexpr reference operator*() const noexcept {
    return {it->first, *it->second};
  }

  [[nodiscard]] constexpr pointer operator->() const noexcept {
    return operator*();
  }

  template<typename Lhs, typename Rhs>
  friend constexpr std::ptrdiff_t operator-(const registry_storage_iterator<Lhs> &,
                                            const registry_storage_iterator<Rhs> &) noexcept;

  template<typename Lhs, typename Rhs>
  friend constexpr bool operator==(const registry_storage_iterator<Lhs> &,
                                   const registry_storage_iterator<Rhs> &) noexcept;

  template<typename Lhs, typename Rhs>
  friend constexpr bool operator<(const registry_storage_iterator<Lhs> &,
                                  const registry_storage_iterator<Rhs> &) noexcept;

 private:
  It it;
};
template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr std::ptrdiff_t operator-(const registry_storage_iterator<Lhs> &lhs,
                                                 const registry_storage_iterator<Rhs> &rhs) noexcept {
  return lhs.it - rhs.it;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator==(const registry_storage_iterator<Lhs> &lhs,
                                        const registry_storage_iterator<Rhs> &rhs) noexcept {
  return lhs.it == rhs.it;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator!=(const registry_storage_iterator<Lhs> &lhs,
                                        const registry_storage_iterator<Rhs> &rhs) noexcept {
  return !(lhs == rhs);
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator<(const registry_storage_iterator<Lhs> &lhs,
                                       const registry_storage_iterator<Rhs> &rhs) noexcept {
  return lhs.it < rhs.it;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator>(const registry_storage_iterator<Lhs> &lhs,
                                       const registry_storage_iterator<Rhs> &rhs) noexcept {
  return rhs < lhs;
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator<=(const registry_storage_iterator<Lhs> &lhs,
                                        const registry_storage_iterator<Rhs> &rhs) noexcept {
  return !(lhs > rhs);
}

template<typename Lhs, typename Rhs>
[[nodiscard]] constexpr bool operator>=(const registry_storage_iterator<Lhs> &lhs,
                                        const registry_storage_iterator<Rhs> &rhs) noexcept {
  return !(lhs < rhs);
}

template<typename Allocator>
class registry_context {
  using alloc_traits = std::allocator_traits<Allocator>;
  using allocator_type = typename alloc_traits::template rebind_alloc<std::pair<const id_type, basic_any<0u>>>;

 public:
  explicit registry_context(const allocator_type &allocator)
      : ctx{allocator} {}

  template<typename Type, typename... Args>
  Type &emplace_as(const id_type id, Args &&...args) {
    return any_cast<Type &>(ctx.try_emplace(id,
                                            std::in_place_type<Type>,
                                            std::forward<Args>(args)...).first->second);
  }

  template<typename Type, typename... Args>
  Type &emplace(Args &&...args) {
    return emplace_as<Type>(type_id<Type>().hash(), std::forward<Args>(args)...);
  }

  template<typename Type>
  Type &insert_or_assign(const id_type id, Type &&value) {
    return any_cast<std::remove_cv_t<std::remove_reference_t<Type >> &
    >(ctx.insert_or_assign(id, std::forward<Type>(value)).first->second);
  }

  template<typename Type>
  Type &insert_or_assign(Type &&value) {
    return insert_or_assign(type_id<Type>().hash(), std::forward<Type>(value));
  }

  template<typename Type>
  bool erase(const id_type id = type_id<Type>().hash()) {
    const auto it = ctx.find(id);
    return it != ctx.end() && it->second.type() == type_id<Type>() ? (ctx.erase(it), true) : false;
  }

  template<typename Type>
  [[nodiscard]] const Type &get(const id_type id = type_id<Type>().hash()) const {
    return any_cast<const Type &>(ctx.at(id));
  }

  template<typename Type>
  [[nodiscard]] Type &get(const id_type id = type_id<Type>().hash()) {
    return any_cast<Type &>(ctx.at(id));
  }

  template<typename Type>
  [[nodiscard]] const Type *find(const id_type id = type_id<Type>().hash()) const {
    const auto it = ctx.find(id);
    return it != ctx.cend() ? any_cast<const Type>(&it->second) : nullptr;
  }

  template<typename Type>
  [[nodiscard]] Type *find(const id_type id = type_id<Type>().hash()) {
    const auto it = ctx.find(id);
    return it != ctx.end() ? any_cast<Type>(&it->second) : nullptr;
  }

  template<typename Type>
  [[nodiscard]] bool contains(const id_type id = type_id<Type>().hash()) const {
    const auto it = ctx.find(id);
    return it != ctx.cend() && it->second.type() == type_id<Type>();
  }

 private:
  dense_map<id_type, basic_any<0u>, identity, std::equal_to<id_type>, allocator_type> ctx;
};

} //namespace internal

/**
 * @brief Fast and reliable entity-component system.
 * @tparam Entity A valid entity type.
 * @tparam Allocator Type of allocator used to manage memory and elements.
 */
template<typename Entity, typename Allocator>
class basic_registry {
  using basic_common_type = basic_sparse_set<Entity, Allocator>;

  using alloc_traits = std::allocator_traits<Allocator>;
  static_assert(std::is_same_v<typename alloc_traits::value_type, Entity>, "Invalid value type");
  // std::shared_ptr because of its type erased allocator which is useful here
  using container_type = dense_map<id_type,
                                   std::shared_ptr<basic_common_type>,
                                   identity,
                                   std::equal_to<id_type>,
                                   typename alloc_traits::template rebind_alloc<std::pair<const id_type,
                                                                                          std::shared_ptr<
                                                                                              basic_common_type>>>>;

  template<typename Type>
  using storage_for_type = typename storage_for<Type, Entity, typename alloc_traits::
  template rebind_alloc<std::remove_const_t<Type>>>::type;

  template<typename...>
  struct group_handler;

  template<typename... Exclude, typename... Get, typename... Owned>
  struct group_handler<exclude_t<Exclude...>, get_t<Get...>, Owned...> {
    // nasty workaround for an issue with the toolset v141 that doesn't accept a fold expression here
    static_assert(!std::disjunction_v<std::bool_constant<storage_for_type<Owned>::traits_type::in_place_delete
    > ...>, "Groups do not support in-place delete");
    using value_type = std::conditional_t<sizeof...(Owned) == 0, basic_common_type, std::size_t>;
    value_type current{};

    template<typename... Args>
    group_handler(Args &&...
    args)
        : current{std::forward<Args>(args)...} {}

    template<typename Type>
    void maybe_valid_if(basic_registry &owner, const Entity entity) {
      [[maybe_unused]] const auto cpools = std::forward_as_tuple(owner.storage<Owned>()...);

      const auto is_valid = ((std::is_same_v<Type, Owned>
          || std::get<storage_for_type<Owned> &>(cpools).contains(entity)) && ...)
          && ((std::is_same_v<Type, Get> || owner.storage<Get>().contains(entity)) && ...)
          && ((std::is_same_v<Type, Exclude> || !owner.storage<Exclude>().contains(entity)) && ...);

      if constexpr (sizeof...(Owned) == 0) {
        if (is_valid && !current.contains(entity)) {
          current.push(entity);
        }
      } else {
        if (is_valid && !(std::get<0>(cpools).index(entity) < current)) {
          const auto pos = current++;
          (std::get<storage_for_type<Owned> &
          >(cpools).swap_elements(std::get<storage_for_type<Owned> &>(cpools).data()[pos], entity), ...);
        }
      }
    }

    void discard_if([[maybe_unused]] basic_registry &owner, const Entity entity) {
      if constexpr (sizeof...(Owned) == 0) {
        current.remove(entity);
      } else {
        if (const auto cpools = std::forward_as_tuple(owner.storage<Owned>()...); std::get<0>(cpools).contains(entity)
            && (std::get<0>(cpools).index(entity) < current)) {
          const auto pos = --current;
          (std::get<storage_for_type<Owned> &
          >(cpools).swap_elements(std::get<storage_for_type<Owned> &>(cpools).data()[pos], entity), ...);
        }
      }
    }
  };

  struct group_data {
    std::size_t size;
    std::shared_ptr<void> group;
    bool (*owned)(const id_type) noexcept;
    bool (*get)(const id_type) noexcept;
    bool (*exclude)(const id_type) noexcept;
  };

  template<typename Type>
  [[nodiscard]] auto &assure(const id_type id = type_hash<Type>::value()) {
    static_assert(std::is_same_v<Type, std::decay_t<Type >>, "Non-decayed types not allowed");
    auto &cpool = pools[id];

    if (!cpool) {
      using alloc_type = typename storage_for_type<std::remove_const_t<Type>>
      ::allocator_type;

      if constexpr (std::is_same_v<Type, void> && !std::is_constructible_v<alloc_type, allocator_type>) {
        // std::allocator<void> has no cross constructors (waiting for C++20)
        cpool =
            std::allocate_shared<storage_for_type<std::remove_const_t<Type>>>(get_allocator(), alloc_type{});
      } else {
        cpool =
            std::allocate_shared<storage_for_type<std::remove_const_t<Type>>>(get_allocator(), get_allocator());
      }

      cpool->bind(forward_as_any(*this));
    }

    RD_ASSERT(cpool->type() == type_id<Type>(), "Unexpected type");
    return static_cast<storage_for_type<Type> &>(*cpool);
  }

  template<typename Type>
  [[nodiscard]] const auto &assure(const id_type id = type_hash<Type>::value()) const {
    static_assert(std::is_same_v<Type, std::decay_t<Type >>, "Non-decayed types not allowed");

    if (const auto it = pools.find(id); it != pools.cend()) {
      RD_ASSERT(it->second->type() == type_id<Type>(), "Unexpected type");
      return static_cast<const storage_for_type<Type> &>(*it->second);
    }

    static storage_for_type<Type> placeholder{};
    return placeholder;
  }

  void rebind() {
    entities->bind(forward_as_any(*this));

    for (auto &&curr : pools) {
      curr.second->bind(forward_as_any(*this));
    }
  }

 public:
  /*! @brief Entity traits. */
  using traits_type = entity_traits<Entity>;
  /*! @brief Allocator type. */
  using allocator_type = Allocator;
  /*! @brief Underlying entity identifier. */
  using entity_type = Entity;
  /*! @brief Underlying version type. */
  using version_type = typename traits_type::version_type;
  /*! @brief Unsigned integer type. */
  using size_type = std::size_t;
  /*! @brief Common type among all storage types. */
  using base_type = basic_common_type;
  /*! @brief Context type. */
  using context = internal::registry_context<allocator_type>;

  /*! @brief Default constructor. */
  basic_registry()
      : basic_registry{allocator_type{}} {}

  /**
   * @brief Constructs an empty registry with a given allocator.
   * @param allocator The allocator to use.
   */
  explicit basic_registry(const allocator_type &allocator)
      : basic_registry{0u, allocator} {}

  /**
   * @brief Allocates enough memory upon construction to store `count` pools.
   * @param count The number of pools to allocate memory for.
   * @param allocator The allocator to use.
   */
  basic_registry(const size_type count, const allocator_type &allocator = allocator_type{})
      : vars{allocator},
        pools{allocator},
        entities{
            std::allocate_shared<storage_for_type<entity_type >>
                (allocator, allocator)
        },
        groups{allocator} {
    pools.reserve(count);
    rebind();
  }

  /**
   * @brief Move constructor.
   * @param other The instance to move from.
   */
  basic_registry(basic_registry &&other) noexcept
      : vars{std::move(other.vars)},
        pools{std::move(other.pools)},
        entities{std::move(other.entities)},
        groups{std::move(other.groups)} {
    rebind();
  }

  /**
   * @brief Move assignment operator.
   * @param other The instance to move from.
   * @return This registry.
   */
  basic_registry &operator=(basic_registry &&other) noexcept {
    vars = std::move(other.vars);
    pools = std::move(other.pools);
    entities = std::move(other.entities);
    groups = std::move(other.groups);

    rebind();

    return *this;
  }

  /**
   * @brief Exchanges the contents with those of a given registry.
   * @param other Registry to exchange the content with.
   */
  void swap(basic_registry &other) {
    using std::swap;

    swap(vars, other.vars);
    swap(pools, other.pools);
    swap(entities, other.entities);
    swap(groups, other.groups);

    rebind();
    other.rebind();
  }

  /**
   * @brief Returns the associated allocator.
   * @return The associated allocator.
   */
  [[nodiscard]] constexpr allocator_type get_allocator() const noexcept {
    return pools.get_allocator();
  }

  /**
   * @brief Returns an iterable object to use to _visit_ a registry.
   *
   * The iterable object returns a pair that contains the name and a reference
   * to the current storage.
   *
   * @return An iterable object to use to _visit_ the registry.
   */
  [[nodiscard]] auto storage() noexcept {
    return iterable_adaptor{internal::registry_storage_iterator{pools.begin()},
                            internal::registry_storage_iterator{pools.end()}};
  }

  /*! @copydoc storage */
  [[nodiscard]] auto storage() const noexcept {
    return iterable_adaptor{internal::registry_storage_iterator{pools.cbegin()},
                            internal::registry_storage_iterator{pools.cend()}};
  }

  /**
   * @brief Finds the storage associated with a given name, if any.
   * @param id Name used to map the storage within the registry.
   * @return A pointer to the storage if it exists, a null pointer otherwise.
   */
  [[nodiscard]] base_type *storage(const id_type id) {
    return const_cast<base_type *>(std::as_const(*this).storage(id));
  }

  /**
   * @brief Finds the storage associated with a given name, if any.
   * @param id Name used to map the storage within the registry.
   * @return A pointer to the storage if it exists, a null pointer otherwise.
   */
  [[nodiscard]] const base_type *storage(const id_type id) const {
    const auto it = pools.find(id);
    return it == pools.cend() ? nullptr : it->second.get();
  }

  /**
   * @brief Returns the storage for a given component type.
   * @tparam Type Type of component of which to return the storage.
   * @param id Optional name used to map the storage within the registry.
   * @return The storage for the given component type.
   */
  template<typename Type>
  decltype(auto) storage(const id_type id = type_hash<Type>::value()) {
    return assure<Type>(id);
  }

  /**
   * @brief Returns the storage for a given component type.
   *
   * @warning
   * If a storage for the given component doesn't exist yet, a temporary
   * placeholder is returned instead.
   *
   * @tparam Type Type of component of which to return the storage.
   * @param id Optional name used to map the storage within the registry.
   * @return The storage for the given component type.
   */
  template<typename Type>
  decltype(auto) storage(const id_type id = type_hash<Type>::value()) const {
    return assure<Type>(id);
  }

  /**
   * @brief Returns the number of entities created so far.
   * @return Number of entities created so far.
   */
  [[nodiscard]] size_type size() const noexcept {
    return entities->size();
  }

  /**
   * @brief Returns the number of entities still in use.
   * @return Number of entities still in use.
   */
  [[nodiscard]] size_type alive() const {
    return entities->in_use();
  }

  /**
   * @brief Increases the capacity (number of entities) of the registry.
   * @param cap Desired capacity.
   */
  void reserve(const size_type cap) {
    entities->reserve(cap);
  }

  /**
   * @brief Returns the number of entities that a registry has currently
   * allocated space for.
   * @return Capacity of the registry.
   */
  [[nodiscard]] size_type capacity() const noexcept {
    return entities->capacity();
  }

  /**
   * @brief Checks whether the registry is empty (no entities still in use).
   * @return True if the registry is empty, false otherwise.
   */
  [[nodiscard]] bool empty() const {
    return !alive();
  }

  /**
   * @brief Direct access to the list of entities of a registry.
   *
   * The returned pointer is such that range `[data(), data() + size())` is
   * always a valid range, even if the registry is empty.
   *
   * @warning
   * This list contains both valid and destroyed entities and isn't suitable
   * for direct use.
   *
   * @return A pointer to the array of entities.
   */
  [[nodiscard]] const entity_type *data() const noexcept {
    return entities->data();
  }

  /**
   * @brief Returns the number of released entities.
   * @return The number of released entities.
   */
  [[nodiscard]] size_type released() const noexcept {
    return (entities->size() - entities->in_use());
  }

  /**
   * @brief Checks if an identifier refers to a valid entity.
   * @param entity An identifier, either valid or not.
   * @return True if the identifier is valid, false otherwise.
   */
  [[nodiscard]] bool valid(const entity_type entity) const {
    return entities->contains(entity);
  }

  /**
   * @brief Returns the actual version for an identifier.
   * @param entity A valid identifier.
   * @return The version for the given identifier if valid, the tombstone
   * version otherwise.
   */
  [[nodiscard]] version_type current(const entity_type entity) const {
    return entities->current(entity);
  }

  /**
   * @brief Creates a new entity or recycles a destroyed one.
   * @return A valid identifier.
   */
  [[nodiscard]] entity_type create() {
    return entities->spawn();
  }

  /**
   * @copybrief create
   *
   * If the requested entity isn't in use, the suggested identifier is used.
   * Otherwise, a new identifier is generated.
   *
   * @param hint Required identifier.
   * @return A valid identifier.
   */
  [[nodiscard]] entity_type create(const entity_type hint) {
    return entities->spawn(hint);
  }

  /**
   * @brief Assigns each element in a range an identifier.
   *
   * @sa create
   *
   * @tparam It Type of forward iterator.
   * @param first An iterator to the first element of the range to generate.
   * @param last An iterator past the last element of the range to generate.
   */
  template<typename It>
  void create(It first, It last) {
    entities->spawn(std::move(first), std::move(last));
  }

  /**
   * @brief Assigns identifiers to an empty registry.
   *
   * This function is intended for use in conjunction with `data`, `size` and
   * `released`.<br/>
   * Don't try to inject ranges of randomly generated entities nor the _wrong_
   * head for the list of destroyed entities. There is no guarantee that a
   * registry will continue to work properly in this case.
   *
   * @warning
   * There must be no entities still alive for this to work properly.
   *
   * @tparam It Type of input iterator.
   * @param first An iterator to the first element of the range of entities.
   * @param last An iterator past the last element of the range of entities.
   * @param destroyed The number of released entities.
   */
  template<typename It>
  void assign(It first, It last, const size_type destroyed) {
    RD_ASSERT(!entities->in_use(), "Non-empty registry");
    entities->push(first, last);
    entities->in_use(entities->size() - destroyed);
  }

  /**
   * @brief Releases an identifier.
   *
   * The version is updated and the identifier can be recycled at any time.
   *
   * @warning
   * Attempting to use an invalid entity results in undefined behavior.
   *
   * @param entity A valid identifier.
   * @return The version of the recycled entity.
   */
  version_type release(const entity_type entity) {
    RD_ASSERT(orphan(entity), "Non-orphan entity");
    entities->erase(entity);
    return entities->current(entity);
  }

  /**
   * @brief Releases an identifier.
   *
   * The suggested version or the valid version closest to the suggested one
   * is used instead of the implicitly generated version.
   *
   * @sa release
   *
   * @param entity A valid identifier.
   * @param version A desired version upon destruction.
   * @return The version actually assigned to the entity.
   */
  version_type release(const entity_type entity, const version_type version) {
    release(entity);
    const auto vers = static_cast<version_type>(version + (version == traits_type::to_version(tombstone)));
    entities->bump(traits_type::construct(traits_type::to_entity(entity), vers));
    return vers;
  }

  /**
   * @brief Releases all identifiers in a range.
   *
   * @sa release
   *
   * @tparam It Type of input iterator.
   * @param first An iterator to the first element of the range of entities.
   * @param last An iterator past the last element of the range of entities.
   */
  template<typename It>
  void release(It first, It last) {
    entities->erase(std::move(first), std::move(last));
  }

  /**
   * @brief Destroys an entity and releases its identifier.
   *
   * @sa release
   *
   * @warning
   * Adding or removing components to an entity that is being destroyed can
   * result in undefined behavior. Attempting to use an invalid entity results
   * in undefined behavior.
   *
   * @param entity A valid identifier.
   * @return The version of the recycled entity.
   */
  version_type destroy(const entity_type entity) {
    return destroy(entity, static_cast<version_type>(traits_type::to_version(entity) + 1u));
  }

  /**
   * @brief Destroys an entity and releases its identifier.
   *
   * The suggested version or the valid version closest to the suggested one
   * is used instead of the implicitly generated version.
   *
   * @sa destroy
   *
   * @param entity A valid identifier.
   * @param version A desired version upon destruction.
   * @return The version actually assigned to the entity.
   */
  version_type destroy(const entity_type entity, const version_type version) {
    for (size_type pos = pools.size(); pos; --pos) {
      pools.begin()[pos - 1u].second->remove(entity);
    }

    return release(entity, version);
  }

  /**
   * @brief Destroys all entities in a range and releases their identifiers.
   *
   * @sa destroy
   *
   * @tparam It Type of input iterator.
   * @param first An iterator to the first element of the range of entities.
   * @param last An iterator past the last element of the range of entities.
   */
  template<typename It>
  void destroy(It first, It last) {
    const auto len = entities->pack(first, last);
    auto from = entities->each().cbegin().base();
    const auto to = from + len;

    for (size_type pos = pools.size(); pos; --pos) {
      pools.begin()[pos - 1u].second->remove(from, to);
    }

    entities->erase(from, to);
  }

  /**
   * @brief Assigns the given component to an entity.
   *
   * The component must have a proper constructor or be of aggregate type.
   *
   * @warning
   * Attempting to assign a component to an entity that already owns it
   * results in undefined behavior.
   *
   * @tparam Type Type of component to create.
   * @tparam Args Types of arguments to use to construct the component.
   * @param entity A valid identifier.
   * @param args Parameters to use to initialize the component.
   * @return A reference to the newly created component.
   */
  template<typename Type, typename... Args>
  decltype(auto) emplace(const entity_type entity, Args &&...args) {
    return assure<Type>().emplace(entity, std::forward<Args>(args)...);
  }

  /**
   * @brief Assigns each entity in a range the given component.
   *
   * @sa emplace
   *
   * @tparam Type Type of component to create.
   * @tparam It Type of input iterator.
   * @param first An iterator to the first element of the range of entities.
   * @param last An iterator past the last element of the range of entities.
   * @param value An instance of the component to assign.
   */
  template<typename Type, typename It>
  void insert(It first, It last, const Type &value = {}) {
    assure<Type>().insert(std::move(first), std::move(last), value);
  }

  /**
   * @brief Assigns each entity in a range the given components.
   *
   * @sa emplace
   *
   * @tparam Type Type of component to create.
   * @tparam EIt Type of input iterator.
   * @tparam CIt Type of input iterator.
   * @param first An iterator to the first element of the range of entities.
   * @param last An iterator past the last element of the range of entities.
   * @param from An iterator to the first element of the range of components.
   */
  template<typename Type, typename EIt, typename CIt, typename = std::enable_if_t<std::is_same_v<typename std::iterator_traits<
      CIt>::value_type, Type>>>
  void insert(EIt first, EIt last, CIt from) {
    assure<Type>().insert(first, last, from);
  }

  /**
   * @brief Assigns or replaces the given component for an entity.
   *
   * @sa emplace
   * @sa replace
   *
   * @tparam Type Type of component to assign or replace.
   * @tparam Args Types of arguments to use to construct the component.
   * @param entity A valid identifier.
   * @param args Parameters to use to initialize the component.
   * @return A reference to the newly created component.
   */
  template<typename Type, typename... Args>
  decltype(auto) emplace_or_replace(const entity_type entity, Args &&...args) {
    if (auto &cpool = assure<Type>(); cpool.contains(entity)) {
      return cpool.patch(entity, [&args...](auto &...curr) { ((curr = Type{std::forward<Args>(args)...}), ...); });
    } else {
      return cpool.emplace(entity, std::forward<Args>(args)...);
    }
  }

  /**
   * @brief Patches the given component for an entity.
   *
   * The signature of the function should be equivalent to the following:
   *
   * @code{.cpp}
   * void(Type &);
   * @endcode
   *
   * @note
   * Empty types aren't explicitly instantiated and therefore they are never
   * returned. However, this function can be used to trigger an update signal
   * for them.
   *
   * @warning
   * Attempting to to patch a component of an entity that doesn't own it
   * results in undefined behavior.
   *
   * @tparam Type Type of component to patch.
   * @tparam Func Types of the function objects to invoke.
   * @param entity A valid identifier.
   * @param func Valid function objects.
   * @return A reference to the patched component.
   */
  template<typename Type, typename... Func>
  decltype(auto) patch(const entity_type entity, Func &&...func) {
    return assure<Type>().patch(entity, std::forward<Func>(func)...);
  }

  /**
   * @brief Replaces the given component for an entity.
   *
   * The component must have a proper constructor or be of aggregate type.
   *
   * @warning
   * Attempting to replace a component of an entity that doesn't own it
   * results in undefined behavior.
   *
   * @tparam Type Type of component to replace.
   * @tparam Args Types of arguments to use to construct the component.
   * @param entity A valid identifier.
   * @param args Parameters to use to initialize the component.
   * @return A reference to the component being replaced.
   */
  template<typename Type, typename... Args>
  decltype(auto) replace(const entity_type entity, Args &&...args) {
    return patch<Type>(entity, [&args...](auto &...curr) { ((curr = Type{std::forward<Args>(args)...}), ...); });
  }

  /**
   * @brief Removes the given components from an entity.
   *
   * @tparam Type Type of component to remove.
   * @tparam Other Other types of components to remove.
   * @param entity A valid identifier.
   * @return The number of components actually removed.
   */
  template<typename Type, typename... Other>
  size_type remove(const entity_type entity) {
    return (assure<Type>().remove(entity) + ... + assure<Other>().remove(entity));
  }

  /**
   * @brief Removes the given components from all the entities in a range.
   *
   * @sa remove
   *
   * @tparam Type Type of component to remove.
   * @tparam Other Other types of components to remove.
   * @tparam It Type of input iterator.
   * @param first An iterator to the first element of the range of entities.
   * @param last An iterator past the last element of the range of entities.
   * @return The number of components actually removed.
   */
  template<typename Type, typename... Other, typename It>
  size_type remove(It first, It last) {
    size_type count{};

    if constexpr (sizeof...(Other) == 0u) {
      count += assure<Type>().remove(std::move(first), std::move(last));
    } else if constexpr (std::is_same_v<It, typename base_type::iterator>) {
      constexpr size_type len = sizeof...(Other) + 1u;
      base_type *cpools[len]{&assure<Type>(), &assure<Other>()...};

      for (size_type pos{}; pos < len; ++pos) {
        if (cpools[pos]->data() == first.data()) {
          std::swap(cpools[pos], cpools[len - 1u]);
        }

        count += cpools[pos]->remove(first, last);
      }
    } else {
      for (auto cpools = std::forward_as_tuple(assure<Type>(), assure<Other>()...); first != last; ++first) {
        count += std::apply([entity = *first](auto &...curr) { return (curr.remove(entity) + ... + 0u); }, cpools);
      }
    }

    return count;
  }

  /**
   * @brief Erases the given components from an entity.
   *
   * @warning
   * Attempting to erase a component from an entity that doesn't own it
   * results in undefined behavior.
   *
   * @tparam Type Types of components to erase.
   * @tparam Other Other types of components to erase.
   * @param entity A valid identifier.
   */
  template<typename Type, typename... Other>
  void erase(const entity_type entity) {
    (assure<Type>().erase(entity), (assure<Other>().erase(entity), ...));
  }

  /**
   * @brief Erases the given components from all the entities in a range.
   *
   * @sa erase
   *
   * @tparam Type Types of components to erase.
   * @tparam Other Other types of components to erase.
   * @tparam It Type of input iterator.
   * @param first An iterator to the first element of the range of entities.
   * @param last An iterator past the last element of the range of entities.
   */
  template<typename Type, typename... Other, typename It>
  void erase(It first, It last) {
    if constexpr (sizeof...(Other) == 0u) {
      assure<Type>().erase(std::move(first), std::move(last));
    } else if constexpr (std::is_same_v<It, typename base_type::iterator>) {
      constexpr size_type len = sizeof...(Other) + 1u;
      base_type *cpools[len]{&assure<Type>(), &assure<Other>()...};

      for (size_type pos{}; pos < len; ++pos) {
        if (cpools[pos]->data() == first.data()) {
          std::swap(cpools[pos], cpools[len - 1u]);
        }

        cpools[pos]->erase(first, last);
      }
    } else {
      for (auto cpools = std::forward_as_tuple(assure<Type>(), assure<Other>()...); first != last; ++first) {
        std::apply([entity = *first](auto &...curr) { (curr.erase(entity), ...); }, cpools);
      }
    }
  }

  /**
   * @brief Removes all tombstones from a registry or only the pools for the
   * given components.
   * @tparam Type Types of components for which to clear all tombstones.
   */
  template<typename... Type>
  void compact() {
    if constexpr (sizeof...(Type) == 0) {
      for (auto &&curr : pools) {
        curr.second->compact();
      }
    } else {
      (assure<Type>().compact(), ...);
    }
  }

  /**
   * @brief Check if an entity is part of all the given storage.
   * @tparam Type Type of storage to check for.
   * @param entity A valid identifier.
   * @return True if the entity is part of all the storage, false otherwise.
   */
  template<typename... Type>
  [[nodiscard]] bool all_of(const entity_type entity) const {
    return (assure<std::remove_const_t<Type >>().contains(entity) && ...);
  }

  /**
   * @brief Check if an entity is part of at least one given storage.
   * @tparam Type Type of storage to check for.
   * @param entity A valid identifier.
   * @return True if the entity is part of at least one storage, false
   * otherwise.
   */
  template<typename... Type>
  [[nodiscard]] bool any_of(const entity_type entity) const {
    return (assure<std::remove_const_t<Type >>().contains(entity) || ...);
  }

  /**
   * @brief Returns references to the given components for an entity.
   *
   * @warning
   * Attempting to get a component from an entity that doesn't own it results
   * in undefined behavior.
   *
   * @tparam Type Types of components to get.
   * @param entity A valid identifier.
   * @return References to the components owned by the entity.
   */
  template<typename... Type>
  [[nodiscard]] decltype(auto) get([[maybe_unused]] const entity_type entity) const {
    if constexpr (sizeof...(Type) == 1u) {
      return (assure<std::remove_const_t<Type >>().get(entity), ...);
    } else {
      return std::forward_as_tuple(get<Type>(entity)...);
    }
  }

  /*! @copydoc get */
  template<typename... Type>
  [[nodiscard]] decltype(auto) get([[maybe_unused]] const entity_type entity) {
    if constexpr (sizeof...(Type) == 1u) {
      return (const_cast<Type &>(std::as_const(*this).template get<Type>(entity)), ...);
    } else {
      return std::forward_as_tuple(get<Type>(entity)...);
    }
  }

  /**
   * @brief Returns a reference to the given component for an entity.
   *
   * In case the entity doesn't own the component, the parameters provided are
   * used to construct it.
   *
   * @sa get
   * @sa emplace
   *
   * @tparam Type Type of component to get.
   * @tparam Args Types of arguments to use to construct the component.
   * @param entity A valid identifier.
   * @param args Parameters to use to initialize the component.
   * @return Reference to the component owned by the entity.
   */
  template<typename Type, typename... Args>
  [[nodiscard]] decltype(auto) get_or_emplace(const entity_type entity, Args &&...args) {
    if (auto &cpool = assure<Type>(); cpool.contains(entity)) {
      return cpool.get(entity);
    } else {
      return cpool.emplace(entity, std::forward<Args>(args)...);
    }
  }

  /**
   * @brief Returns pointers to the given components for an entity.
   *
   * @note
   * The registry retains ownership of the pointed-to components.
   *
   * @tparam Type Types of components to get.
   * @param entity A valid identifier.
   * @return Pointers to the components owned by the entity.
   */
  template<typename... Type>
  [[nodiscard]] auto try_get([[maybe_unused]] const entity_type entity) const {
    if constexpr (sizeof...(Type) == 1) {
      const auto &cpool = assure<std::remove_const_t<Type>...>();
      return cpool.contains(entity) ? std::addressof(cpool.get(entity)) : nullptr;
    } else {
      return std::make_tuple(try_get<Type>(entity)...);
    }
  }

  /*! @copydoc try_get */
  template<typename... Type>
  [[nodiscard]] auto try_get([[maybe_unused]] const entity_type entity) {
    if constexpr (sizeof...(Type) == 1) {
      return (const_cast<Type *>(std::as_const(*this).template try_get<Type>(entity)), ...);
    } else {
      return std::make_tuple(try_get<Type>(entity)...);
    }
  }

  /**
   * @brief Clears a whole registry or the pools for the given components.
   * @tparam Type Types of components to remove from their entities.
   */
  template<typename... Type>
  void clear() {
    if constexpr (sizeof...(Type) == 0) {
      for (size_type pos = pools.size(); pos; --pos) {
        pools.begin()[pos - 1u].second->clear();
      }

      auto iterable = entities->each();
      entities->erase(iterable.begin().base(), iterable.end().base());
    } else {
      (assure<Type>().clear(), ...);
    }
  }

  /**
   * @brief Iterates all the entities that are still in use.
   *
   * The signature of the function should be equivalent to the following:
   *
   * @code{.cpp}
   * void(const Entity);
   * @endcode
   *
   * It's not defined whether entities created during iteration are returned.
   *
   * @tparam Func Type of the function object to invoke.
   * @param func A valid function object.
   */
  template<typename Func>
  void each(Func func) const {
    for (auto [entity] : entities->each()) {
      func(entity);
    }
  }

  /**
   * @brief Checks if an entity has components assigned.
   * @param entity A valid identifier.
   * @return True if the entity has no components assigned, false otherwise.
   */
  [[nodiscard]] bool orphan(const entity_type entity) const {
    return std::none_of(pools.cbegin(), pools.cend(), [entity](auto &&curr) { return curr.second->contains(entity); });
  }

  /**
   * @brief Returns a sink object for the given component.
   *
   * Use this function to receive notifications whenever a new instance of the
   * given component is created and assigned to an entity.<br/>
   * The function type for a listener is equivalent to:
   *
   * @code{.cpp}
   * void(basic_registry<Entity> &, Entity);
   * @endcode
   *
   * Listeners are invoked **after** assigning the component to the entity.
   *
   * @sa sink
   *
   * @tparam Type Type of component of which to get the sink.
   * @return A temporary sink object.
   */
  template<typename Type>
  [[nodiscard]] auto on_construct() {
    return assure<Type>().on_construct();
  }

  /**
   * @brief Returns a sink object for the given component.
   *
   * Use this function to receive notifications whenever an instance of the
   * given component is explicitly updated.<br/>
   * The function type for a listener is equivalent to:
   *
   * @code{.cpp}
   * void(basic_registry<Entity> &, Entity);
   * @endcode
   *
   * Listeners are invoked **after** updating the component.
   *
   * @sa sink
   *
   * @tparam Type Type of component of which to get the sink.
   * @return A temporary sink object.
   */
  template<typename Type>
  [[nodiscard]] auto on_update() {
    return assure<Type>().on_update();
  }

  /**
   * @brief Returns a sink object for the given component.
   *
   * Use this function to receive notifications whenever an instance of the
   * given component is removed from an entity and thus destroyed.<br/>
   * The function type for a listener is equivalent to:
   *
   * @code{.cpp}
   * void(basic_registry<Entity> &, Entity);
   * @endcode
   *
   * Listeners are invoked **before** removing the component from the entity.
   *
   * @sa sink
   *
   * @tparam Type Type of component of which to get the sink.
   * @return A temporary sink object.
   */
  template<typename Type>
  [[nodiscard]] auto on_destroy() {
    return assure<Type>().on_destroy();
  }

  /**
   * @brief Returns a view for the given components.
   *
   * Views are created on the fly and share with the registry its internal
   * data structures. Feel free to discard them after the use.<br/>
   * Creating and destroying a view is an incredibly cheap operation. As a
   * rule of thumb, storing a view should never be an option.
   *
   * @tparam Type Type of component used to construct the view.
   * @tparam Other Other types of components used to construct the view.
   * @tparam Exclude Types of components used to filter the view.
   * @return A newly created view.
   */
  template<typename Type, typename... Other, typename... Exclude>
  [[nodiscard]] basic_view<get_t<storage_for_type<const Type>, storage_for_type<const Other>...>, exclude_t<
      storage_for_type<const Exclude>...>>
  view(
      exclude_t<Exclude...> = {}
  ) const {
    return {assure<std::remove_const_t<Type >>(), assure<std::remove_const_t<Other >>()...,
            assure<std::remove_const_t<Exclude >>()...};
  }

  /*! @copydoc view */
  template<typename Type, typename... Other, typename... Exclude>
  [[nodiscard]] basic_view<get_t<storage_for_type<Type>,
                                 storage_for_type<Other>...>, exclude_t<storage_for_type<Exclude>...>>
  view(
      exclude_t<Exclude...> = {}
  ) {
    return {assure<std::remove_const_t<Type >>(), assure<std::remove_const_t<Other >>()...,
            assure<std::remove_const_t<Exclude >>()...};
  }

  /**
   * @brief Returns a group for the given components.
   *
   * Groups are created on the fly and share with the registry its internal
   * data structures. Feel free to discard them after the use.<br/>
   * Creating and destroying a group is an incredibly cheap operation. As a
   * rule of thumb, storing a group should never be an option.
   *
   * Groups support exclusion lists and can own types of components. The more
   * types are owned by a group, the faster it is to iterate entities and
   * components.<br/>
   * However, groups also affect some features of the registry such as the
   * creation and destruction of components.
   *
   * @note
   * Pools of components that are owned by a group cannot be sorted anymore.
   * The group takes the ownership of the pools and arrange components so as
   * to iterate them as fast as possible.
   *
   * @tparam Owned Type of storage _owned_ by the group.
   * @tparam Get Type of storage _observed_ by the group.
   * @tparam Exclude Type of storage used to filter the group.
   * @return A newly created group.
   */
  template<typename... Owned, typename... Get, typename... Exclude>
  [[nodiscard]] basic_group<owned_t<storage_for_type<Owned>...>, get_t<storage_for_type<Get>...>, exclude_t<
      storage_for_type<Exclude>...>>
  group(
      get_t<Get...> = {}, exclude_t<Exclude...> = {}
  ) {
    static_assert(sizeof...(Owned) + sizeof...(Get) > 0, "Exclusion-only groups are not supported");
    static_assert(sizeof...(Owned) + sizeof...(Get) + sizeof...(Exclude) > 1,
                  "Single component groups are not allowed");

    using handler_type = group_handler<exclude_t<std::remove_const_t<Exclude>...>,
                                       get_t<std::remove_const_t
                                                 <Get>...>,
                                       std::remove_const_t<Owned>...>;

    const auto cpools = std::forward_as_tuple(assure<std::remove_const_t<Owned >>()...,
                                              assure<std::remove_const_t<Get >>()...);
    constexpr auto size = sizeof...(Owned) + sizeof...(Get) + sizeof...(Exclude);
    handler_type *handler = nullptr;

    auto it = std::find_if(groups.cbegin(), groups.cend(), [size](const auto &gdata) {
      return gdata.size == size
          && (gdata.owned(type_hash<std::remove_const_t<Owned >>::value()) && ...)
          && (gdata.get(type_hash<std::remove_const_t<Get >>::value()) && ...)
          && (gdata.exclude(type_hash<std::remove_const_t<Exclude >>::value()) && ...);
    });

    if (it != groups.cend()) {
      handler = static_cast<handler_type *>(it->group.get());
    } else {
      group_data candidate = {
          size,
          std::apply([this](auto &&...args) {
            return std::allocate_shared<handler_type>(get_allocator(),
                                                      std::forward<decltype(args)>(args)...);
          }, rendu::uses_allocator_construction_args<typename handler_type::value_type>(get_allocator())),
          []([[maybe_unused]] const id_type ctype) noexcept {
            return ((ctype == type_hash<std::remove_const_t<Owned >>::value()) || ...);
          },
          []([[maybe_unused]] const id_type ctype) noexcept {
            return ((ctype == type_hash<std::remove_const_t<Get >>::value()) || ...);
          },
          []([[maybe_unused]] const id_type ctype) noexcept {
            return ((ctype == type_hash<std::remove_const_t<Exclude >>::value()) || ...);
          },
      };

      handler = static_cast<handler_type *>(candidate.group.get());

      const void *maybe_valid_if = nullptr;
      const void *discard_if = nullptr;

      if constexpr (sizeof...(Owned) == 0) {
        groups.push_back(std::move(candidate));
      } else {
        [[maybe_unused]] auto has_conflict = [size](const auto &gdata) {
          const auto overlapping = (0u + ... + gdata.owned(type_hash<std::remove_const_t<Owned >>::value()));
          const auto sz = overlapping + (0u + ... + gdata.get(type_hash<std::remove_const_t<Get >>::value()))
              + (0u + ... + gdata.exclude(type_hash<std::remove_const_t<Exclude >>::value()));
          return !overlapping || ((sz == size) || (sz == gdata.size));
        };

        RD_ASSERT(std::all_of(groups.cbegin(), groups.cend(), std::move(has_conflict)), "Conflicting groups");

        const auto next = std::find_if_not(groups.cbegin(), groups.cend(), [size](const auto &gdata) {
          return !(0u + ... + gdata.owned(type_hash<std::remove_const_t<Owned >>::value()))
              || (size > gdata.size);
        });

        const auto prev = std::find_if(std::make_reverse_iterator(next), groups.crend(), [](const auto &gdata) {
          return (0u + ... + gdata.owned(type_hash<std::remove_const_t<Owned >>::value()));
        });

        maybe_valid_if = (next == groups.cend() ? maybe_valid_if : next->group.get());
        discard_if = (prev == groups.crend() ? discard_if : prev->group.get());
        groups.insert(next, std::move(candidate));
      }

      (on_construct<std::remove_const_t<Owned
      >>().before(maybe_valid_if).template connect<&handler_type::template maybe_valid_if<
          std::remove_const_t<Owned>>>(*handler), ...);
      (on_construct<std::remove_const_t<Get
      >>().before(maybe_valid_if).template connect<&handler_type::template maybe_valid_if<
          std::remove_const_t<Get>>>(*handler), ...);
      (on_destroy<std::remove_const_t<Exclude
      >>().before(maybe_valid_if).template connect<&handler_type::template maybe_valid_if<
          std::remove_const_t<Exclude>>>(*handler), ...);

      (on_destroy<std::remove_const_t
                      <Owned >>().before(discard_if).template connect<&handler_type::discard_if>(*handler), ...);
      (on_destroy<std::remove_const_t
                      <Get >>().before(discard_if).template connect<&handler_type::discard_if>(*handler), ...);
      (on_construct<std::remove_const_t
                        <Exclude >>().before(discard_if).template connect<&handler_type::discard_if>(*handler), ...);

      if constexpr (sizeof...(Owned) == 0) {
        for (const auto entity : view<Owned..., Get...>(exclude<Exclude...>)) {
          handler->current.push(entity);
        }
      } else {
        // we cannot iterate backwards because we want to leave behind valid entities in case of owned types
        for (auto *first = std::get<0>(cpools).data(), *last = first + std::get<0>(cpools).size(); first != last;
             ++first) {
          handler->template maybe_valid_if<type_list_element_t<0,
                                                               type_list<std::remove_const_t<Owned>...>>>(*this,
                                                                                                          *first);
        }
      }
    }

    return {handler->current, std::get<storage_for_type<std::remove_const_t<Owned >> &>(cpools)...,
            std::get<storage_for_type<std::remove_const_t<Get >> &>(cpools)...,
            assure<std::remove_const_t<Exclude >>()...};
  }

  /*! @copydoc group */
  template<typename... Owned, typename... Get, typename... Exclude>
  [[nodiscard]] basic_group<
      owned_t<storage_for_type<const Owned>...>, get_t<storage_for_type<const Get>...>, exclude_t<
          storage_for_type<const Exclude>...>>
  group_if_exists(
      get_t<Get...> = {}, exclude_t<Exclude...> = {}
  ) const {
    auto it = std::find_if(groups.cbegin(), groups.cend(), [](const auto &gdata) {
      return gdata.size == (sizeof...(Owned) + sizeof...(Get) + sizeof...(Exclude))
          && (gdata.owned(type_hash<std::remove_const_t<Owned >>::value()) && ...)
          && (gdata.get(type_hash<std::remove_const_t<Get >>::value()) && ...)
          && (gdata.exclude(type_hash<std::remove_const_t<Exclude >>::value()) && ...);
    });

    if (it == groups.cend()) {
      return {};
    } else {
      using handler_type = group_handler<exclude_t<std::remove_const_t<Exclude>...>,
                                         get_t<std::remove_const_t
                                                   <Get>...>,
                                         std::remove_const_t<Owned>...>;
      return {static_cast<handler_type *>(it->group.get())->current, assure<std::remove_const_t<Owned >>()...,
              assure<std::remove_const_t<Get >>()..., assure<std::remove_const_t<Exclude >>()...};
    }
  }

  /**
   * @brief Checks whether the given components belong to any group.
   * @tparam Type Types of components in which one is interested.
   * @return True if the pools of the given components are _free_, false
   * otherwise.
   */
  template<typename... Type>
  [[nodiscard]] bool owned() const {
    return std::any_of(groups.cbegin(),
                       groups.cend(),
                       [](auto &&gdata) {
                         return (gdata.owned(type_hash<std::remove_const_t<Type >>::value()) || ...);
                       });
  }

  /**
   * @brief Checks whether a group can be sorted.
   * @tparam Owned Type of storage _owned_ by the group.
   * @tparam Get Type of storage _observed_ by the group.
   * @tparam Exclude Type of storage used to filter the group.
   * @return True if the group can be sorted, false otherwise.
   */
  template<typename... Owned, typename... Get, typename... Exclude>
  [[nodiscard]] bool sortable(const basic_group<owned_t<Owned...>, get_t<Get...>, exclude_t<Exclude...>>
                              &) noexcept {
    constexpr auto size = sizeof...(Owned) + sizeof...(Get) + sizeof...(Exclude);
    auto pred = [size](const auto &gdata) {
      return (0u + ... + gdata.owned(type_hash<typename Owned::value_type>::value())) && (size < gdata.size);
    };
    return std::find_if(groups.cbegin(), groups.cend(), std::move(pred)) == groups.cend();
  }

  /**
   * @brief Sorts the elements of a given component.
   *
   * The order remains valid until a component of the given type is assigned
   * to or removed from an entity.<br/>
   * The comparison function object returns `true` if the first element is
   * _less_ than the second one, `false` otherwise. Its signature is also
   * equivalent to one of the following:
   *
   * @code{.cpp}
   * bool(const Entity, const Entity);
   * bool(const Type &, const Type &);
   * @endcode
   *
   * Moreover, it shall induce a _strict weak ordering_ on the values.<br/>
   * The sort function object offers an `operator()` that accepts:
   *
   * * An iterator to the first element of the range to sort.
   * * An iterator past the last element of the range to sort.
   * * A comparison function object to use to compare the elements.
   *
   * The comparison function object hasn't necessarily the type of the one
   * passed along with the other parameters to this member function.
   *
   * @warning
   * Pools of components owned by a group cannot be sorted.
   *
   * @tparam Type Type of components to sort.
   * @tparam Compare Type of comparison function object.
   * @tparam Sort Type of sort function object.
   * @tparam Args Types of arguments to forward to the sort function object.
   * @param compare A valid comparison function object.
   * @param algo A valid sort function object.
   * @param args Arguments to forward to the sort function object, if any.
   */
  template<typename Type, typename Compare, typename Sort = std_sort, typename... Args>
  void sort(Compare compare, Sort algo = Sort{}, Args &&...args) {
    RD_ASSERT(!owned<Type>(), "Cannot sort owned storage");
    auto &cpool = assure<Type>();

    if constexpr (std::is_invocable_v<Compare, decltype(cpool.get({})), decltype(cpool.get({}))>) {
      auto comp = [&cpool, compare = std::move(compare)](const auto lhs,
                                                         const auto rhs) {
        return compare(std::as_const(cpool.get(lhs)),
                       std::as_const(cpool.get(rhs)));
      };
      cpool.sort(std::move(comp), std::move(algo), std::forward<Args>(args)...);
    } else {
      cpool.sort(std::move(compare), std::move(algo), std::forward<Args>(args)...);
    }
  }

  /**
   * @brief Sorts two pools of components in the same way.
   *
   * Being `To` and `From` the two sets, after invoking this function an
   * iterator for `To` returns elements according to the following rules:
   *
   * * All entities in `To` that are also in `From` are returned first
   *   according to the order they have in `From`.
   * * All entities in `To` that are not in `From` are returned in no
   *   particular order after all the other entities.
   *
   * Any subsequent change to `From` won't affect the order in `To`.
   *
   * @warning
   * Pools of components owned by a group cannot be sorted.
   *
   * @tparam To Type of components to sort.
   * @tparam From Type of components to use to sort.
   */
  template<typename To, typename From>
  void sort() {
    RD_ASSERT(!owned<To>(), "Cannot sort owned storage");
    assure<To>().respect(assure<From>());
  }

  /**
   * @brief Returns the context object, that is, a general purpose container.
   * @return The context object, that is, a general purpose container.
   */
  context &ctx() noexcept {
    return vars;
  }

  /*! @copydoc ctx */
  const context &ctx() const noexcept {
    return vars;
  }

 private:
  context vars;
  container_type pools;
  std::shared_ptr<storage_for_type<entity_type>> entities;
  std::vector<group_data, typename alloc_traits::
  template rebind_alloc<group_data>> groups;
};

} //namespace rendu

#endif //RENDU_CORE_ECS_REGISTRY_H_