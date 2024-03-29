/*
* Created by boil on 2023/2/19.
*/
#include <array>
#include <vector>
#include <test/rdtest.h>
#include <core/base/algorithm.h>

struct boxed_int {
  int value;
};

TEST(Algorithm, StdSort) {
  // well, I'm pretty sure it works, it's std::sort!!
  std::array<int, 5> arr{{4, 1, 3, 2, 0}};
  rendu::std_sort sort;

  sort(arr.begin(), arr.end());

  for(auto i = 0u; i < (arr.size() - 1u); ++i) {
    ASSERT_LT(arr[i], arr[i + 1u]);
  }
}

TEST(Algorithm, StdSortBoxedInt) {
  // well, I'm pretty sure it works, it's std::sort!!
  std::array<boxed_int, 6> arr{{{4}, {1}, {3}, {2}, {0}, {6}}};
  rendu::std_sort sort;

  sort(arr.begin(), arr.end(), [](const auto &lhs, const auto &rhs) {
    return lhs.value > rhs.value;
  });

  for(auto i = 0u; i < (arr.size() - 1u); ++i) {
    ASSERT_GT(arr[i].value, arr[i + 1u].value);
  }
}

TEST(Algorithm, InsertionSort) {
  std::array<int, 5> arr{{4, 1, 3, 2, 0}};
  rendu::insertion_sort sort;

  sort(arr.begin(), arr.end());

  for(auto i = 0u; i < (arr.size() - 1u); ++i) {
    ASSERT_LT(arr[i], arr[i + 1u]);
  }
}

TEST(Algorithm, InsertionSortBoxedInt) {
  std::array<boxed_int, 6> arr{{{4}, {1}, {3}, {2}, {0}, {6}}};
  rendu::insertion_sort sort;

  sort(arr.begin(), arr.end(), [](const auto &lhs, const auto &rhs) {
    return lhs.value > rhs.value;
  });

  for(auto i = 0u; i < (arr.size() - 1u); ++i) {
    ASSERT_GT(arr[i].value, arr[i + 1u].value);
  }
}

TEST(Algorithm, InsertionSortEmptyContainer) {
  std::vector<int> vec{};
  rendu::insertion_sort sort;
  // this should crash with asan enabled if we break the constraint
  sort(vec.begin(), vec.end());
}

TEST(Algorithm, RadixSort) {
  std::array<uint32_t, 5> arr{{4, 1, 3, 2, 0}};
  rendu::radix_sort<8, 32> sort;

  sort(arr.begin(), arr.end(), [](const auto &value) {
    return value;
  });

  for(auto i = 0u; i < (arr.size() - 1u); ++i) {
    ASSERT_LT(arr[i], arr[i + 1u]);
  }
}

TEST(Algorithm, RadixSortBoxedInt) {
  std::array<boxed_int, 6> arr{{{4}, {1}, {3}, {2}, {0}, {6}}};
  rendu::radix_sort<2, 6> sort;

  sort(arr.rbegin(), arr.rend(), [](const auto &instance) {
    return instance.value;
  });

  for(auto i = 0u; i < (arr.size() - 1u); ++i) {
    ASSERT_GT(arr[i].value, arr[i + 1u].value);
  }
}

TEST(Algorithm, RadixSortEmptyContainer) {
  std::vector<int> vec{};
  rendu::radix_sort<8, 32> sort;
  // this should crash with asan enabled if we break the constraint
  sort(vec.begin(), vec.end());
}
