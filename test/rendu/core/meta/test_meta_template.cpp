/*
* Created by boil on 2023/2/26.
*/

#include <test/rdtest.h>
#include "test_meta_fwd.h"

namespace test::meta_template {

TEST(MetaTemplate, Invalid) {
  const auto type = rendu::resolve<int>();

  ASSERT_FALSE(type.is_template_specialization());
  ASSERT_EQ(type.template_arity(), 0u);
  ASSERT_EQ(type.template_type(), rendu::meta_type{});
  ASSERT_EQ(type.template_arg(0u), rendu::meta_type{});
}

TEST(MetaTemplate, Valid) {
  const auto type = rendu::resolve<rendu::type_list<int, char>>();

  ASSERT_TRUE(type.is_template_specialization());
  ASSERT_EQ(type.template_arity(), 2u);
  ASSERT_EQ(type.template_type(), rendu::resolve<rendu::meta_class_template_tag<rendu::type_list>>());
  ASSERT_EQ(type.template_arg(0u), rendu::resolve<int>());
  ASSERT_EQ(type.template_arg(1u), rendu::resolve<char>());
  ASSERT_EQ(type.template_arg(2u), rendu::meta_type{});
}

TEST(MetaTemplate, CustomTraits) {
  const auto type = rendu::resolve<function_type<void(int, const char &)>>();

  ASSERT_TRUE(type.is_template_specialization());
  ASSERT_EQ(type.template_arity(), 3u);
  ASSERT_EQ(type.template_type(), rendu::resolve<rendu::meta_class_template_tag<function_type>>());
  ASSERT_EQ(type.template_arg(0u), rendu::resolve<void>());
  ASSERT_EQ(type.template_arg(1u), rendu::resolve<int>());
  ASSERT_EQ(type.template_arg(2u), rendu::resolve<char>());
  ASSERT_EQ(type.template_arg(3u), rendu::meta_type{});
}

}
