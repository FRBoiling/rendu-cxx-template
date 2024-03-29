/*
* Created by boil on 2023/2/19.
*/

#include <test/rdtest.h>
#include <core/base/iterator.h>

struct clazz {
  int value{0};
};

TEST(InputIteratorPointer, Functionalities) {
  clazz instance{};
  rendu::input_iterator_pointer ptr{std::move(instance)};
  ptr->value = 42;

  ASSERT_EQ(instance.value, 0);
  ASSERT_EQ(ptr->value, 42);
  ASSERT_EQ(ptr->value, (*ptr).value);
  ASSERT_EQ(ptr.operator->(), &ptr.operator*());
}

TEST(IotaIterator, Functionalities) {
  rendu::iota_iterator<std::size_t> first{};
  const rendu::iota_iterator<std::size_t> last{2u};

  ASSERT_NE(first, last);
  ASSERT_FALSE(first == last);
  ASSERT_TRUE(first != last);

  ASSERT_EQ(*first++, 0u);
  ASSERT_EQ(*first, 1u);
  ASSERT_EQ(*++first, *last);
  ASSERT_EQ(*first, 2u);
}

TEST(IterableAdaptor, Functionalities) {
  std::vector<int> vec{1, 2};
  rendu::iterable_adaptor iterable{vec.begin(), vec.end()};
  decltype(iterable) other{};

  ASSERT_NO_FATAL_FAILURE(other = iterable);
  ASSERT_NO_FATAL_FAILURE(std::swap(other, iterable));

  ASSERT_EQ(iterable.begin(), vec.begin());
  ASSERT_EQ(iterable.end(), vec.end());

  ASSERT_EQ(*iterable.cbegin(), 1);
  ASSERT_EQ(*++iterable.cbegin(), 2);
  ASSERT_EQ(++iterable.cbegin(), --iterable.end());

  for (auto value : rendu::iterable_adaptor<const int *, const void *>{vec.data(), vec.data() + 1u}) {
    ASSERT_EQ(value, 1);
  }
}

