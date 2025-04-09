/*
* Created by boil on 2022/10/2.
*/

#include "catch/rd_catch2.hpp"
#include <set>

TEST_CASE("Set插入测试", "[set][insert]") {
    std::set<int> s;

    SECTION("插入单个元素") {
        s.insert(42);
        REQUIRE(s.size() == 1);
        REQUIRE(s.count(42) == 1); // 元素存在
    }

    SECTION("插入重复元素") {
        s.insert(10);
        s.insert(10); // 重复插入
        REQUIRE(s.size() == 1); // 集合中只有一个
    }

    SECTION("插入多个元素") {
        s.insert({3, 1, 2});
        REQUIRE(s.size() == 3);
        REQUIRE(s.count(1) == 1);
        REQUIRE(s.count(2) == 1);
        REQUIRE(s.count(3) == 1);
    }
}

TEST_CASE("Set删除测试", "[set][erase]") {
    std::set<int> s = {1, 2, 3, 4};

    SECTION("删除存在的元素") {
        s.erase(2);
        REQUIRE(s.size() == 3);
        REQUIRE(s.count(2) == 0); // 元素不存在
    }

    SECTION("删除不存在的元素") {
        s.erase(99);
        REQUIRE(s.size() == 4); // 大小不变
    }

    SECTION("清空集合") {
        s.clear();
        REQUIRE(s.empty());
    }
}

TEST_CASE("Set查找测试", "[set][find]") {
    std::set<int> s = {5, 10, 15};

    SECTION("查找存在的元素") {
        auto it = s.find(10);
        REQUIRE(it != s.end()); // 找到元素
        REQUIRE(*it == 10);
    }

    SECTION("查找不存在的元素") {
        auto it = s.find(20);
        REQUIRE(it == s.end()); // 未找到
    }

    SECTION("下界和上界") {
        REQUIRE(*s.lower_bound(8) == 10); // 第一个>=8的元素
        REQUIRE(*s.upper_bound(10) == 15); // 第一个>10的元素
    }
}

TEST_CASE("Set顺序测试", "[set][order]") {
    std::set<int> s = {3, 1, 4, 1, 5}; // 插入顺序: 3,1,4,1,5

    // 验证元素按升序排列
    std::vector<int> expected = {1, 3, 4, 5};
    std::vector<int> actual(s.begin(), s.end());

    REQUIRE(actual == expected);
}

TEST_CASE("Set自定义排序测试", "[set][comparator]") {
    // 使用greater<int>实现降序排列
    std::set<int, std::greater<int>> s = {3, 1, 4};

    std::vector<int> expected = {4, 3, 1};
    std::vector<int> actual(s.begin(), s.end());

    REQUIRE(actual == expected);
}

// 辅助函数：计算两个集合的交集
template<typename T>
std::set<T> set_intersection(const std::set<T>& a, const std::set<T>& b) {
    std::set<T> result;
    std::set_intersection(a.begin(), a.end(),
                          b.begin(), b.end(),
                          std::inserter(result, result.begin()));
    return result;
}

TEST_CASE("Set交集测试", "[set][operation]") {
    std::set<int> a = {1, 2, 3};
    std::set<int> b = {2, 3, 4};

    std::set<int> intersection = set_intersection(a, b);
    REQUIRE(intersection == std::set<int>{2, 3});
}

struct NonCopyable {
    int value;
    NonCopyable(int v) : value(v) {}
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = default;
    bool operator<(const NonCopyable& other) const {
        return value < other.value;
    }
};

TEST_CASE("Set移动语义测试", "[set][move]") {
    std::set<NonCopyable> s;
    s.emplace(42); // 使用emplace避免拷贝
    REQUIRE(s.size() == 1);
}