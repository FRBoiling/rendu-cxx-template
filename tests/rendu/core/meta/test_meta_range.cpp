/*
* Created by boil on 2023/2/27.
*/
#include <test/rdtest.h>
#include <core/base//hashed_string.h>
#include <core/locator/locator.h>
#include <core/meta/factory.h>
#include <core/meta/meta.h>
#include <core/meta/range.h>
#include <core/meta/resolve.h>

namespace test::meta::range {

struct MetaRange : ::testing::Test {
  void SetUp() override {
    using namespace rendu::literals;

    rendu::meta<int>().type("int"_hs).data<42>("answer"_hs);
  }

  void TearDown() override {
    rendu::meta_reset();
  }
};

TEST_F(MetaRange, EmptyRange) {
  rendu::meta_reset();
  auto range = rendu::resolve();
  ASSERT_EQ(range.begin(), range.end());
}

TEST_F(MetaRange, Iterator) {
  using namespace rendu::literals;

  using iterator = typename decltype(rendu::resolve())::iterator;

  static_assert(std::is_same_v<iterator::value_type, std::pair<rendu::id_type, rendu::meta_type>>);
  static_assert(std::is_same_v<iterator::pointer,
                               rendu::input_iterator_pointer<std::pair<rendu::id_type, rendu::meta_type>>>);
  static_assert(std::is_same_v<iterator::reference, std::pair<rendu::id_type, rendu::meta_type>>);

  auto range = rendu::resolve();

  iterator end{range.begin()};
  iterator begin{};
  begin = range.end();
  std::swap(begin, end);

  ASSERT_EQ(begin, range.begin());
  ASSERT_EQ(end, range.end());
  ASSERT_NE(begin, end);

  ASSERT_EQ(begin++, range.begin());
  ASSERT_EQ(begin--, range.end());

  ASSERT_EQ(begin + 1, range.end());
  ASSERT_EQ(end - 1, range.begin());

  ASSERT_EQ(++begin, range.end());
  ASSERT_EQ(--begin, range.begin());

  ASSERT_EQ(begin += 1, range.end());
  ASSERT_EQ(begin -= 1, range.begin());

  ASSERT_EQ(begin + (end - begin), range.end());
  ASSERT_EQ(begin - (begin - end), range.end());

  ASSERT_EQ(end - (end - begin), range.begin());
  ASSERT_EQ(end + (begin - end), range.begin());

  ASSERT_EQ(begin[0u].first, range.begin()->first);
  ASSERT_EQ(begin[0u].second, (*range.begin()).second);

  ASSERT_LT(begin, end);
  ASSERT_LE(begin, range.begin());

  ASSERT_GT(end, begin);
  ASSERT_GE(end, range.end());

  rendu::meta<double>().type("double"_hs);
  range = rendu::resolve();
  begin = range.begin();

  ASSERT_EQ(begin[0u].first, rendu::resolve<int>().info().hash());
  ASSERT_EQ(begin[1u].second, rendu::resolve("double"_hs));
}

TEST_F(MetaRange, DirectValue) {
  using namespace rendu::literals;

  auto type = rendu::resolve<int>();
  auto range = type.data();

  ASSERT_NE(range.cbegin(), range.cend());

  for (auto &&[id, data] : range) {
    ASSERT_EQ(id, "answer"_hs);
    ASSERT_EQ(data.get({}).cast<int>(), 42);
  }
}
}
