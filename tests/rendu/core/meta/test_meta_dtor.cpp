/*
* Created by boil on 2023/2/27.
*/
#include <test/rdtest.h>
#include <core/base/hashed_string.h>
#include <core/locator/locator.h>
#include <core/meta/factory.h>
#include <core/meta/meta.h>
#include <core/meta/node.h>
#include <core/meta/resolve.h>

namespace test::meta::dtor {

struct clazz_t {
  clazz_t() {
    ++counter;
  }

  static void destroy_decr(clazz_t &) {
    --counter;
  }

  void destroy_incr() const {
    ++counter;
  }

  inline static int counter = 0;
};

struct MetaDtor : ::testing::Test {
  void SetUp() override {
    using namespace rendu::literals;

    rendu::meta<clazz_t>()
        .type("clazz"_hs)
        .dtor<clazz_t::destroy_decr>();

    clazz_t::counter = 0;
  }

  void TearDown() override {
    rendu::meta_reset();
  }
};

TEST_F(MetaDtor, Functionalities) {
  ASSERT_EQ(clazz_t::counter, 0);

  auto any = rendu::resolve<clazz_t>().construct();
  auto cref = std::as_const(any).as_ref();
  auto ref = any.as_ref();

  ASSERT_TRUE(any);
  ASSERT_TRUE(cref);
  ASSERT_TRUE(ref);

  ASSERT_EQ(clazz_t::counter, 1);

  cref.reset();
  ref.reset();

  ASSERT_TRUE(any);
  ASSERT_FALSE(cref);
  ASSERT_FALSE(ref);

  ASSERT_EQ(clazz_t::counter, 1);

  any.reset();

  ASSERT_FALSE(any);
  ASSERT_FALSE(cref);
  ASSERT_FALSE(ref);

  ASSERT_EQ(clazz_t::counter, 0);
}

TEST_F(MetaDtor, AsRefConstruction) {
  ASSERT_EQ(clazz_t::counter, 0);

  clazz_t instance{};
  auto any = rendu::forward_as_meta(instance);
  auto cany = rendu::forward_as_meta(std::as_const(instance));
  auto cref = cany.as_ref();
  auto ref = any.as_ref();

  ASSERT_TRUE(any);
  ASSERT_TRUE(cany);
  ASSERT_TRUE(cref);
  ASSERT_TRUE(ref);

  ASSERT_EQ(clazz_t::counter, 1);

  any.reset();
  cany.reset();
  cref.reset();
  ref.reset();

  ASSERT_FALSE(any);
  ASSERT_FALSE(cany);
  ASSERT_FALSE(cref);
  ASSERT_FALSE(ref);

  ASSERT_EQ(clazz_t::counter, 1);
}

TEST_F(MetaDtor, ReRegistration) {
  SetUp();

  auto &&node =
      rendu::internal::resolve<clazz_t>(rendu::internal::meta_context::from(rendu::locator<rendu::meta_ctx>::value_or()));

  ASSERT_NE(node.dtor.dtor, nullptr);

  rendu::meta<clazz_t>().dtor<&clazz_t::destroy_incr>();
  rendu::resolve<clazz_t>().construct().reset();

  ASSERT_EQ(clazz_t::counter, 2);
}
}
