//
// Created by boil on 2023/2/17.
//

#ifndef RENDU_CORE_ECS_MIXIN_H_

#include "base/any.h"
#include "signal/sigh.h"
#include "entity.h"
#include "fwd.h"

namespace rendu {

/**
 * @brief Mixin type used to add signal support to storage types.
 *
 * The function type of a listener is equivalent to:
 *
 * @code{.cpp}
 * void(basic_registry<entity_type> &, entity_type);
 * @endcode
 *
 * This applies to all signals made available.
 *
 * @tparam Type The type of the underlying storage.
 */
template<typename Type>
class sigh_mixin final : public Type {
  using basic_registry_type = basic_registry<typename Type::entity_type, typename Type::base_type::allocator_type>;
  using sigh_type = sigh<void(basic_registry_type &, const typename Type::entity_type), typename Type::allocator_type>;
  using underlying_iterator = typename Type::base_type::basic_iterator;

  void pop(underlying_iterator first, underlying_iterator last) final {
    if (destruction.empty()) {
      Type::pop(first, last);
    } else {
      RD_ASSERT(owner != nullptr, "Invalid pointer to registry");

      for (; first != last; ++first) {
        const auto rendu = *first;
        destruction.publish(*owner, rendu);
        const auto it = Type::find(rendu);
        Type::pop(it, it + 1u);
      }
    }
  }

  void pop_all() final {
    if (!destruction.empty()) {
      RD_ASSERT(owner != nullptr, "Invalid pointer to registry");

      for (auto pos = Type::each().begin().base().index(); !(pos < 0); --pos) {
        if constexpr (Type::traits_type::in_place_delete) {
          if (const auto rendu = Type::operator[](static_cast<typename Type::size_type>(pos)); rendu != tombstone) {
            destruction.publish(*owner, rendu);
          }
        } else {
          destruction.publish(*owner, Type::operator[](static_cast<typename Type::size_type>(pos)));
        }
      }
    }

    Type::pop_all();
  }

  underlying_iterator try_emplace(const typename Type::entity_type rendu,
                                  const bool force_back,
                                  const void *value) final {
    const auto it = Type::try_emplace(rendu, force_back, value);

    if (it != Type::base_type::end()) {
      RD_ASSERT(owner != nullptr, "Invalid pointer to registry");
      construction.publish(*owner, *it);
    }

    return it;
  }

 public:
  /*! @brief Allocator type. */
  using allocator_type = typename Type::allocator_type;
  /*! @brief Underlying entity identifier. */
  using entity_type = typename Type::entity_type;
  /*! @brief Expected registry type. */
  using registry_type = basic_registry_type;

  /*! @brief Default constructor. */
  sigh_mixin()
      : sigh_mixin{allocator_type{}} {}

  /**
   * @brief Constructs an empty storage with a given allocator.
   * @param allocator The allocator to use.
   */
  explicit sigh_mixin(const allocator_type &allocator)
      : Type{allocator},
        owner{},
        construction{allocator},
        destruction{allocator},
        update{allocator} {}

  /**
   * @brief Move constructor.
   * @param other The instance to move from.
   */
  sigh_mixin(sigh_mixin &&other) noexcept
      : Type{std::move(other)},
        owner{other.owner},
        construction{std::move(other.construction)},
        destruction{std::move(other.destruction)},
        update{std::move(other.update)} {}

  /**
   * @brief Allocator-extended move constructor.
   * @param other The instance to move from.
   * @param allocator The allocator to use.
   */
  sigh_mixin(sigh_mixin &&other, const allocator_type &allocator) noexcept
      : Type{std::move(other), allocator},
        owner{other.owner},
        construction{std::move(other.construction), allocator},
        destruction{std::move(other.destruction), allocator},
        update{std::move(other.update), allocator} {}

  /**
   * @brief Move assignment operator.
   * @param other The instance to move from.
   * @return This storage.
   */
  sigh_mixin &operator=(sigh_mixin &&other) noexcept {
    Type::operator=(std::move(other));
    owner = other.owner;
    construction = std::move(other.construction);
    destruction = std::move(other.destruction);
    update = std::move(other.update);
    return *this;
  }

  /**
   * @brief Exchanges the contents with those of a given storage.
   * @param other Storage to exchange the content with.
   */
  void swap(sigh_mixin &other) {
    using std::swap;
    Type::swap(other);
    swap(owner, other.owner);
    swap(construction, other.construction);
    swap(destruction, other.destruction);
    swap(update, other.update);
  }

  /**
   * @brief Returns a sink object.
   *
   * The sink returned by this function can be used to receive notifications
   * whenever a new instance is created and assigned to an entity.<br/>
   * Listeners are invoked after the object has been assigned to the entity.
   *
   * @sa sink
   *
   * @return A temporary sink object.
   */
  [[nodiscard]] auto on_construct() noexcept {
    return sink{construction};
  }

  /**
   * @brief Returns a sink object.
   *
   * The sink returned by this function can be used to receive notifications
   * whenever an instance is explicitly updated.<br/>
   * Listeners are invoked after the object has been updated.
   *
   * @sa sink
   *
   * @return A temporary sink object.
   */
  [[nodiscard]] auto on_update() noexcept {
    return sink{update};
  }

  /**
   * @brief Returns a sink object.
   *
   * The sink returned by this function can be used to receive notifications
   * whenever an instance is removed from an entity and thus destroyed.<br/>
   * Listeners are invoked before the object has been removed from the entity.
   *
   * @sa sink
   *
   * @return A temporary sink object.
   */
  [[nodiscard]] auto on_destroy() noexcept {
    return sink{destruction};
  }

  /**
   * @brief Assigns entities to a storage.
   * @tparam Args Types of arguments to use to construct the object.
   * @param rendu A valid identifier.
   * @param args Parameters to use to initialize the object.
   * @return A reference to the newly created object.
   */
  template<typename... Args>
  decltype(auto) emplace(const entity_type rendu, Args &&...args) {
    RD_ASSERT(owner != nullptr, "Invalid pointer to registry");
    Type::emplace(rendu, std::forward<Args>(args)...);
    construction.publish(*owner, rendu);
    return this->get(rendu);
  }

  /**
   * @brief Patches the given instance for an entity.
   * @tparam Func Types of the function objects to invoke.
   * @param rendu A valid identifier.
   * @param func Valid function objects.
   * @return A reference to the patched instance.
   */
  template<typename... Func>
  decltype(auto) patch(const entity_type rendu, Func &&...func) {
    RD_ASSERT(owner != nullptr, "Invalid pointer to registry");
    Type::patch(rendu, std::forward<Func>(func)...);
    update.publish(*owner, rendu);
    return this->get(rendu);
  }

  /**
   * @brief Assigns entities to a storage.
   * @tparam It Type of input iterator.
   * @tparam Args Types of arguments to use to construct the objects assigned
   * to the entities.
   * @param first An iterator to the first element of the range of entities.
   * @param last An iterator past the last element of the range of entities.
   * @param args Parameters to use to initialize the objects assigned to the
   * entities.
   */
  template<typename It, typename... Args>
  void insert(It first, It last, Args &&...args) {
    Type::insert(first, last, std::forward<Args>(args)...);

    if (!construction.empty()) {
      RD_ASSERT(owner != nullptr, "Invalid pointer to registry");

      for (; first != last; ++first) {
        construction.publish(*owner, *first);
      }
    }
  }

  /**
   * @brief Creates a new identifier or recycles a destroyed one.
   * @return A valid identifier.
   */
  entity_type spawn() {
    RD_ASSERT(owner != nullptr, "Invalid pointer to registry");
    const auto rendu = Type::spawn();
    construction.publish(*owner, rendu);
    return rendu;
  }

  /**
   * @brief Creates a new identifier or recycles a destroyed one.
   *
   * If the requested identifier isn't in use, the suggested one is used.
   * Otherwise, a new identifier is returned.
   *
   * @param hint Required identifier.
   * @return A valid identifier.
   */
  entity_type spawn(const entity_type hint) {
    RD_ASSERT(owner != nullptr, "Invalid pointer to registry");
    const auto rendu = Type::spawn(hint);
    construction.publish(*owner, rendu);
    return rendu;
  }

  /**
   * @brief Assigns each element in a range an identifier.
   * @tparam It Type of mutable forward iterator.
   * @param first An iterator to the first element of the range to generate.
   * @param last An iterator past the last element of the range to generate.
   */
  template<typename It>
  void spawn(It first, It last) {
    Type::spawn(first, last);

    if (!construction.empty()) {
      RD_ASSERT(owner != nullptr, "Invalid pointer to registry");

      for (; first != last; ++first) {
        construction.publish(*owner, *first);
      }
    }
  }

  /**
   * @brief Forwards variables to derived classes, if any.
   * @param value A variable wrapped in an opaque container.
   */
  void bind(any value) noexcept final {
    auto *reg = any_cast<basic_registry_type>(&value);
    owner = reg ? reg : owner;
    Type::bind(std::move(value));
  }

 private:
  basic_registry_type *owner;
  sigh_type construction;
  sigh_type destruction;
  sigh_type update;
};

} // namespace rendu 


#define RENDU_CORE_ECS_MIXIN_H_

#endif //RENDU_CORE_ECS_MIXIN_H_
