/*
* Created by boil on 2023/2/17.
*/

#include <test/rdtest.h>
#include <core/ecs/entity.h>
#include <core/ecs/registry.h>

namespace test::ecs::entity {

TEST(Entity, Traits) {
  using traits_type = rendu::entity_traits<rendu::entity>;
  constexpr rendu::entity tombstone = rendu::tombstone;
  constexpr rendu::entity null = rendu::null;
  rendu::registry registry{};

  registry.destroy(registry.create());
  const auto entity = registry.create();
  const auto other = registry.create();

  ASSERT_EQ(rendu::to_integral(entity), rendu::to_integral(entity));
  ASSERT_NE(rendu::to_integral(entity), rendu::to_integral<rendu::entity>(rendu::null));
  ASSERT_NE(rendu::to_integral(entity), rendu::to_integral(rendu::entity{}));

  ASSERT_EQ(rendu::to_entity(entity), 0u);
  ASSERT_EQ(rendu::to_version(entity), 1u);
  ASSERT_EQ(rendu::to_entity(other), 1u);
  ASSERT_EQ(rendu::to_version(other), 0u);

  ASSERT_EQ(traits_type::construct(rendu::to_entity(entity), rendu::to_version(entity)), entity);
  ASSERT_EQ(traits_type::construct(rendu::to_entity(other), rendu::to_version(other)), other);
  ASSERT_NE(traits_type::construct(rendu::to_entity(entity), {}), entity);

  ASSERT_EQ(traits_type::construct(rendu::to_entity(other), rendu::to_version(entity)),
            traits_type::combine(rendu::to_integral(other), rendu::to_integral(entity)));

  ASSERT_EQ(traits_type::combine(rendu::tombstone, rendu::null), tombstone);
  ASSERT_EQ(traits_type::combine(rendu::null, rendu::tombstone), null);

  ASSERT_EQ(traits_type::next(entity),
            traits_type::construct(rendu::to_integral(entity), rendu::to_version(entity) + 1u));
  ASSERT_EQ(traits_type::next(other), traits_type::construct(rendu::to_integral(other), rendu::to_version(other) + 1u));

  ASSERT_EQ(traits_type::next(rendu::tombstone), traits_type::construct(rendu::null, {}));
  ASSERT_EQ(traits_type::next(rendu::null), traits_type::construct(rendu::null, {}));
}

TEST(Entity, Null) {
  using traits_type = rendu::entity_traits<rendu::entity>;
  constexpr rendu::entity null = rendu::null;

  ASSERT_FALSE(rendu::entity{} == rendu::null);
  ASSERT_TRUE(rendu::null == rendu::null);
  ASSERT_FALSE(rendu::null != rendu::null);

  rendu::registry registry{};
  const auto entity = registry.create();

  ASSERT_EQ(traits_type::combine(rendu::null, rendu::to_integral(entity)),
            (traits_type::construct(rendu::to_entity(null), rendu::to_version(entity))));
  ASSERT_EQ(traits_type::combine(rendu::null, rendu::to_integral(null)), null);
  ASSERT_EQ(traits_type::combine(rendu::null, rendu::tombstone), null);

  registry.emplace<int>(entity, 42);

  ASSERT_FALSE(entity == rendu::null);
  ASSERT_FALSE(rendu::null == entity);

  ASSERT_TRUE(entity != rendu::null);
  ASSERT_TRUE(rendu::null != entity);

  const rendu::entity other = rendu::null;

  ASSERT_FALSE(registry.valid(other));
  ASSERT_NE(registry.create(other), other);
}

TEST(Entity, Tombstone) {
  using traits_type = rendu::entity_traits<rendu::entity>;
  constexpr rendu::entity tombstone = rendu::tombstone;

  ASSERT_FALSE(rendu::entity{} == rendu::tombstone);
  ASSERT_TRUE(rendu::tombstone == rendu::tombstone);
  ASSERT_FALSE(rendu::tombstone != rendu::tombstone);

  rendu::registry registry{};
  const auto entity = registry.create();

  ASSERT_EQ(traits_type::combine(rendu::to_integral(entity), rendu::tombstone),
            (traits_type::construct(rendu::to_entity(entity), rendu::to_version(tombstone))));
  ASSERT_EQ(traits_type::combine(rendu::tombstone, rendu::to_integral(tombstone)), tombstone);
  ASSERT_EQ(traits_type::combine(rendu::tombstone, rendu::null), tombstone);

  registry.emplace<int>(entity, 42);

  ASSERT_FALSE(entity == rendu::tombstone);
  ASSERT_FALSE(rendu::tombstone == entity);

  ASSERT_TRUE(entity != rendu::tombstone);
  ASSERT_TRUE(rendu::tombstone != entity);

  constexpr auto vers = rendu::to_version(tombstone);
  const auto other = traits_type::construct(rendu::to_entity(entity), vers);

  ASSERT_FALSE(registry.valid(rendu::tombstone));
  ASSERT_NE(registry.destroy(entity, vers), vers);
  ASSERT_NE(registry.create(other), other);
}
}