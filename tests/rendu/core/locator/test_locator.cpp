/*
* Created by boil on 2023/2/25.
*/

#include <test/rdtest.h>
#include <core/locator/locator.h>

namespace test::locator {

struct base_service {
  virtual ~base_service() = default;
  virtual void invoke() {}
};

struct null_service : base_service {
  void invoke() override {
    invoked = true;
  }

  static inline bool invoked{};
};

struct derived_service : base_service {
  void invoke() override {
    invoked = true;
  }

  static inline bool invoked{};
};

struct ServiceLocator : ::testing::Test {
  void SetUp() override {
    null_service::invoked = false;
    derived_service::invoked = false;
  }
};

using ServiceLocatorDeathTest = ServiceLocator;

TEST(ServiceLocator, Functionalities) {
  ASSERT_FALSE(rendu::locator<base_service>::has_value());
  ASSERT_FALSE(derived_service::invoked);
  ASSERT_FALSE(null_service::invoked);

  rendu::locator<base_service>::value_or<null_service>().invoke();

  ASSERT_TRUE(rendu::locator<base_service>::has_value());
  ASSERT_TRUE(null_service::invoked);

  auto handle = rendu::locator<base_service>::handle();
  rendu::locator<base_service>::reset();

  ASSERT_FALSE(rendu::locator<base_service>::has_value());

  rendu::locator<base_service>::reset(handle);

  ASSERT_TRUE(rendu::locator<base_service>::has_value());

  rendu::locator<base_service>::reset(decltype(handle){});

  ASSERT_FALSE(rendu::locator<base_service>::has_value());

  rendu::locator<base_service>::emplace<derived_service>();
  rendu::locator<base_service>::value().invoke();

  ASSERT_TRUE(rendu::locator<base_service>::has_value());
  ASSERT_TRUE(derived_service::invoked);

  derived_service::invoked = false;
  rendu::locator<base_service>::allocate_emplace<derived_service>(std::allocator<derived_service>{}).invoke();

  ASSERT_TRUE(rendu::locator<base_service>::has_value());
  ASSERT_TRUE(derived_service::invoked);
}

RD_DEBUG_TEST(ServiceLocatorDeathTest, UninitializedValue) {
  ASSERT_NO_FATAL_FAILURE(rendu::locator<base_service>::value_or().invoke());

  rendu::locator<base_service>::reset();

  ASSERT_DEATH(rendu::locator<base_service>::value().invoke(), "");
}
}
