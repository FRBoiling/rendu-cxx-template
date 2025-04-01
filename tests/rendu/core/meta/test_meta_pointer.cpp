/*
* Created by boil on 2023/2/26.
*/

#include <test/rdtest.h>
#include "test_meta_fwd.h"

namespace test::meta::pointer {

int test_function() {
  return 42;
}

TEST(MetaPointerLike, DereferenceOperatorInvalidType) {
  int value = 0;
  rendu::meta_any any{value};

  ASSERT_FALSE(any.type().is_pointer());
  ASSERT_FALSE(any.type().is_pointer_like());
  ASSERT_EQ(any.type(), rendu::resolve<int>());

  auto deref = *any;

  ASSERT_FALSE(deref);
}

TEST(MetaPointerLike, DereferenceOperatorConstType) {
  const int value = 42;
  rendu::meta_any any{&value};

  ASSERT_TRUE(any.type().is_pointer());
  ASSERT_TRUE(any.type().is_pointer_like());
  ASSERT_EQ(any.type(), rendu::resolve<const int *>());

  auto deref = *any;

  ASSERT_TRUE(deref);
  ASSERT_FALSE(deref.type().is_pointer());
  ASSERT_FALSE(deref.type().is_pointer_like());
  ASSERT_EQ(deref.type(), rendu::resolve<int>());

  ASSERT_EQ(deref.try_cast<int>(), nullptr);
  ASSERT_EQ(deref.try_cast<const int>(), &value);
  ASSERT_EQ(deref.cast<const int &>(), 42);
}

TEST(MetaPointerLike, DereferenceOperatorConstAnyNonConstType) {
  int value = 42;
  const rendu::meta_any any{&value};
  auto deref = *any;

  ASSERT_TRUE(deref);
  ASSERT_FALSE(deref.type().is_pointer());
  ASSERT_FALSE(deref.type().is_pointer_like());
  ASSERT_EQ(deref.type(), rendu::resolve<int>());

  ASSERT_NE(deref.try_cast<int>(), nullptr);
  ASSERT_NE(deref.try_cast<const int>(), nullptr);
  ASSERT_EQ(deref.cast<int &>(), 42);
  ASSERT_EQ(deref.cast<const int &>(), 42);
}

TEST(MetaPointerLike, DereferenceOperatorConstAnyConstType) {
  const int value = 42;
  const rendu::meta_any any{&value};
  auto deref = *any;

  ASSERT_TRUE(deref);
  ASSERT_FALSE(deref.type().is_pointer());
  ASSERT_FALSE(deref.type().is_pointer_like());
  ASSERT_EQ(deref.type(), rendu::resolve<int>());

  ASSERT_EQ(deref.try_cast<int>(), nullptr);
  ASSERT_NE(deref.try_cast<const int>(), nullptr);
  ASSERT_EQ(deref.cast<const int &>(), 42);
}

TEST(MetaPointerLike, DereferenceOperatorRawPointer) {
  int value = 0;
  rendu::meta_any any{&value};

  ASSERT_TRUE(any.type().is_pointer());
  ASSERT_TRUE(any.type().is_pointer_like());
  ASSERT_EQ(any.type(), rendu::resolve<int *>());

  auto deref = *any;

  ASSERT_TRUE(deref);
  ASSERT_FALSE(deref.type().is_pointer());
  ASSERT_FALSE(deref.type().is_pointer_like());
  ASSERT_EQ(deref.type(), rendu::resolve<int>());

  deref.cast<int &>() = 42;

  ASSERT_EQ(*any.cast<int *>(), 42);
  ASSERT_EQ(value, 42);
}

TEST(MetaPointerLike, DereferenceOperatorSmartPointer) {
  auto value = std::make_shared<int>(0);
  rendu::meta_any any{value};

  ASSERT_FALSE(any.type().is_pointer());
  ASSERT_TRUE(any.type().is_pointer_like());
  ASSERT_EQ(any.type(), rendu::resolve<std::shared_ptr<int>>());

  auto deref = *any;

  ASSERT_TRUE(deref);
  ASSERT_FALSE(deref.type().is_pointer());
  ASSERT_FALSE(deref.type().is_pointer_like());
  ASSERT_EQ(deref.type(), rendu::resolve<int>());

  deref.cast<int &>() = 42;

  ASSERT_EQ(*any.cast<std::shared_ptr<int>>(), 42);
  ASSERT_EQ(*value, 42);
}

TEST(MetaPointerLike, PointerToConstMoveOnlyType) {
  const std::unique_ptr<int> instance;
  rendu::meta_any any{&instance};
  auto deref = *any;

  ASSERT_TRUE(any);
  ASSERT_TRUE(deref);

  ASSERT_EQ(deref.try_cast<std::unique_ptr<int>>(), nullptr);
  ASSERT_NE(deref.try_cast<const std::unique_ptr<int>>(), nullptr);
  ASSERT_EQ(&deref.cast<const std::unique_ptr<int> &>(), &instance);
}

TEST(MetaPointerLike, AsRef) {
  int value = 0;
  int *ptr = &value;
  rendu::meta_any any{rendu::forward_as_meta(ptr)};

  ASSERT_TRUE(any.type().is_pointer());
  ASSERT_TRUE(any.type().is_pointer_like());
  ASSERT_EQ(any.type(), rendu::resolve<int *>());

  auto deref = *any;

  ASSERT_TRUE(deref);
  ASSERT_FALSE(deref.type().is_pointer());
  ASSERT_FALSE(deref.type().is_pointer_like());
  ASSERT_EQ(deref.type(), rendu::resolve<int>());

  deref.cast<int &>() = 42;

  ASSERT_EQ(*any.cast<int *>(), 42);
  ASSERT_EQ(value, 42);
}

TEST(MetaPointerLike, AsConstRef) {
  int value = 42;
  int *const ptr = &value;
  rendu::meta_any any{rendu::forward_as_meta(ptr)};

  ASSERT_TRUE(any.type().is_pointer());
  ASSERT_TRUE(any.type().is_pointer_like());
  ASSERT_EQ(any.type(), rendu::resolve<int *>());

  auto deref = *any;

  ASSERT_TRUE(deref);
  ASSERT_FALSE(deref.type().is_pointer());
  ASSERT_FALSE(deref.type().is_pointer_like());
  ASSERT_EQ(deref.type(), rendu::resolve<int>());

  deref.cast<int &>() = 42;

  ASSERT_EQ(*any.cast<int *>(), 42);
  ASSERT_EQ(value, 42);
}

TEST(MetaPointerLike, DereferenceOverload) {
  auto test = [](rendu::meta_any any) {
    ASSERT_FALSE(any.type().is_pointer());
    ASSERT_TRUE(any.type().is_pointer_like());

    auto deref = *any;

    ASSERT_TRUE(deref);
    ASSERT_FALSE(deref.type().is_pointer());
    ASSERT_FALSE(deref.type().is_pointer_like());
    ASSERT_EQ(deref.type(), rendu::resolve<int>());

    ASSERT_EQ(deref.cast<int &>(), 42);
    ASSERT_EQ(deref.cast<const int &>(), 42);
  };

  test(adl_wrapped_shared_ptr<int>{42});
  test(spec_wrapped_shared_ptr<int>{42});
}

TEST(MetaPointerLike, DereferencePointerToConstOverload) {
  auto test = [](rendu::meta_any any) {
    ASSERT_FALSE(any.type().is_pointer());
    ASSERT_TRUE(any.type().is_pointer_like());

    auto deref = *any;

    ASSERT_TRUE(deref);
    ASSERT_FALSE(deref.type().is_pointer());
    ASSERT_FALSE(deref.type().is_pointer_like());
    ASSERT_EQ(deref.type(), rendu::resolve<int>());
    ASSERT_EQ(deref.cast<const int &>(), 42);
  };

  test(adl_wrapped_shared_ptr<const int>{42});
  test(spec_wrapped_shared_ptr<const int>{42});
}

TEST(MetaPointerLike, DereferencePointerToVoid) {
  auto test = [](rendu::meta_any any) {
    ASSERT_TRUE(any.type().is_pointer());
    ASSERT_TRUE(any.type().is_pointer_like());

    auto deref = *any;

    ASSERT_FALSE(deref);
  };

  test(static_cast<void *>(nullptr));
  test(static_cast<const void *>(nullptr));
}

TEST(MetaPointerLike, DereferenceSmartPointerToVoid) {
  auto test = [](rendu::meta_any any) {
    ASSERT_TRUE(any.type().is_class());
    ASSERT_FALSE(any.type().is_pointer());
    ASSERT_TRUE(any.type().is_pointer_like());

    auto deref = *any;

    ASSERT_FALSE(deref);
  };

  test(std::shared_ptr<void>{});
  test(std::unique_ptr<void, void (*)(void *)>{nullptr, nullptr});
}

TEST(MetaPointerLike, DereferencePointerToFunction) {
  auto test = [](rendu::meta_any any) {
    ASSERT_TRUE(any.type().is_pointer());
    ASSERT_TRUE(any.type().is_pointer_like());
    ASSERT_NE(any.try_cast < int(*)() > (), nullptr);
    ASSERT_EQ(any.cast < int(*)() > ()(), 42);
  };

  rendu::meta_any func{&test_function};

  test(func);
  test(*func);
  test(**func);
  test(*std::as_const(func));
}

TEST(MetaPointerLike, DereferenceSelfPointer) {
  self_ptr obj{42};
  rendu::meta_any any{rendu::forward_as_meta(obj)};
  rendu::meta_any deref = *any;

  ASSERT_TRUE(deref);
  ASSERT_TRUE(any.type().is_pointer_like());
  ASSERT_EQ(deref.cast<const self_ptr &>().value, obj.value);
  ASSERT_FALSE(deref.try_cast<self_ptr>());
}

TEST(MetaPointerLike, DereferenceProxyPointer) {
  int value = 3;
  proxy_ptr obj{value};
  rendu::meta_any any{obj};
  rendu::meta_any deref = *any;

  ASSERT_TRUE(deref);
  ASSERT_TRUE(any.type().is_pointer_like());
  ASSERT_EQ(*deref.cast<const proxy_ptr &>().value, value);
  ASSERT_TRUE(deref.try_cast<proxy_ptr>());

  *deref.cast<proxy_ptr &>().value = 42;

  ASSERT_EQ(value, 42);
}

TEST(MetaPointerLike, DereferenceArray) {
  rendu::meta_any array{std::in_place_type<int[3]>};
  rendu::meta_any array_of_array{std::in_place_type<int[3][3]>};

  ASSERT_EQ(array.type(), rendu::resolve<int[3]>());
  ASSERT_EQ(array_of_array.type(), rendu::resolve<int[3][3]>());

  ASSERT_FALSE(*array);
  ASSERT_FALSE(*array_of_array);
}

TEST(MetaPointerLike, DereferenceVerifiableNullPointerLike) {
  auto test = [](rendu::meta_any any) {
    ASSERT_TRUE(any);
    ASSERT_FALSE(*any);
  };

  test(rendu::meta_any{static_cast<int *>(nullptr)});
  test(rendu::meta_any{std::shared_ptr<int>{}});
  test(rendu::meta_any{std::unique_ptr<int>{}});
}

RD_DEBUG_TEST(MetaPointerLikeDeathTest, DereferenceOperatorConstType) {
  const int value = 42;
  rendu::meta_any any{&value};
  auto deref = *any;

  ASSERT_TRUE(deref);
  ASSERT_DEATH(deref.cast<int &>() = 0, "");
}

RD_DEBUG_TEST(MetaPointerLikeDeathTest, DereferenceOperatorConstAnyConstType) {
  const int value = 42;
  const rendu::meta_any any{&value};
  auto deref = *any;

  ASSERT_TRUE(deref);
  ASSERT_DEATH(deref.cast<int &>() = 0, "");
}

RD_DEBUG_TEST(MetaPointerLikeDeathTest, DereferencePointerToConstOverload) {
  auto test = [](rendu::meta_any any) {
    auto deref = *any;

    ASSERT_TRUE(deref);
    ASSERT_DEATH(deref.cast<int &>() = 42, "");
  };

  test(adl_wrapped_shared_ptr<const int>{42});
  test(spec_wrapped_shared_ptr<const int>{42});
}
}
