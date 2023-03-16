/*
* Created by boil on 2023/2/27.
*/
#include <test/rdtest.h>
#include <core/base/hashed_string.h>
#include <core/locator/locator.h>
#include <core/meta/factory.h>
#include <core/meta/node.h>
#include <core/meta/resolve.h>

namespace test::meta::conv {

struct clazz_t {
  clazz_t() = default;

  operator int() const {
    return value;
  }

  bool to_bool() const {
    return (value != 0);
  }

  int value{};
};

double conv_to_double(const clazz_t &instance) {
  return instance.value * 2.;
}

struct MetaConv : ::testing::Test {
  void SetUp() override {
    using namespace rendu::literals;

    rendu::meta<clazz_t>()
        .type("clazz"_hs)
        .conv<int>()
        .conv<&clazz_t::to_bool>()
        .conv<conv_to_double>();
  }

  void TearDown() override {
    rendu::meta_reset();
  }
};

TEST_F(MetaConv, Functionalities) {
  auto any = rendu::resolve<clazz_t>().construct();
  any.cast<clazz_t &>().value = 42;

  const auto as_int = std::as_const(any).allow_cast<int>();
  const auto as_bool = std::as_const(any).allow_cast<bool>();
  const auto as_double = std::as_const(any).allow_cast<double>();

  ASSERT_FALSE(any.allow_cast<char>());

  ASSERT_TRUE(as_int);
  ASSERT_TRUE(as_bool);
  ASSERT_TRUE(as_double);

  ASSERT_EQ(as_int.cast<int>(), any.cast<clazz_t &>().operator int());
  ASSERT_EQ(as_bool.cast<bool>(), any.cast<clazz_t &>().to_bool());
  ASSERT_EQ(as_double.cast<double>(), conv_to_double(any.cast<clazz_t &>()));
}

TEST_F(MetaConv, ReRegistration) {
  SetUp();

  auto &&node =
      rendu::internal::resolve<clazz_t>(rendu::internal::meta_context::from(rendu::locator<rendu::meta_ctx>::value_or()));

  ASSERT_TRUE(node.details);
  ASSERT_FALSE(node.details->conv.empty());
  ASSERT_EQ(node.details->conv.size(), 3u);
}
}
