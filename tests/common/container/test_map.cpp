/*
* Created by boil on 2022/10/2.
*/
#include "catch/rd_catch2.hpp"
#include <map>

TEST_CASE("Map插入测试", "[map][insert]") {
    std::map<std::string, int> m;

    SECTION("使用insert插入") {
        m.insert({"apple", 1});
        m.insert({"banana", 2});
        REQUIRE(m.size() == 2);
        REQUIRE(m["apple"] == 1);
        REQUIRE(m["banana"] == 2);
    }

    SECTION("使用emplace插入") {
        m.emplace("cherry", 3);
        REQUIRE(m["cherry"] == 3);
    }

    SECTION("插入重复键") {
        m["dog"] = 4;
        m["dog"] = 5; // 覆盖旧值
        REQUIRE(m.size() == 1);
        REQUIRE(m["dog"] == 5);
    }
}

TEST_CASE("Map删除测试", "[map][erase]") {
    std::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};

    SECTION("删除存在的键") {
        m.erase(2);
        REQUIRE(m.size() == 2);
        REQUIRE(m.find(2) == m.end()); // 键不存在
    }

    SECTION("删除不存在的键") {
        m.erase(99);
        REQUIRE(m.size() == 3); // 大小不变
    }

    SECTION("清空映射") {
        m.clear();
        REQUIRE(m.empty());
    }
}

TEST_CASE("Map查找测试", "[map][find]") {
    std::map<std::string, double> m = {{"pi", 3.14}, {"e", 2.718}};

    SECTION("查找存在的键") {
        auto it = m.find("pi");
        REQUIRE(it != m.end()); // 找到键
        REQUIRE(it->second == Catch::Approx(3.14));
    }

    SECTION("查找不存在的键") {
        auto it = m.find("tau");
        REQUIRE(it == m.end()); // 未找到
    }

    SECTION("访问不存在的键") {
        REQUIRE_THROWS_AS(m.at("invalid"), std::out_of_range); // at()抛出异常
        REQUIRE_NOTHROW(m["invalid"]); // []操作符插入默认值
    }
}

TEST_CASE("Map顺序测试", "[map][order]") {
    std::map<int, char> m = {{3, 'c'}, {1, 'a'}, {2, 'b'}}; // 插入顺序: 3,1,2

    // 验证元素按键升序排列
    std::vector<int> expected_keys = {1, 2, 3};
    std::vector<int> actual_keys;
    for (const auto& pair : m) {
        actual_keys.push_back(pair.first);
    }

    REQUIRE(actual_keys == expected_keys);
}

TEST_CASE("Map自定义排序测试", "[map][comparator]") {
    // 使用greater<int>实现键的降序排列
    std::map<int, std::string, std::greater<int>> m = {{1, "one"}, {3, "three"}, {2, "two"}};

    std::vector<int> expected_keys = {3, 2, 1};
    std::vector<int> actual_keys;
    for (const auto& pair : m) {
        actual_keys.push_back(pair.first);
    }

    REQUIRE(actual_keys == expected_keys);
}

TEST_CASE("Map迭代器测试", "[map][iterator]") {
    std::map<char, int> m = {{'a', 1}, {'b', 2}, {'c', 3}};

    SECTION("正向迭代") {
        std::string result;
        for (const auto& pair : m) {
            result += pair.first;
        }
        REQUIRE(result == "abc");
    }

    SECTION("反向迭代") {
        std::string result;
        for (auto it = m.rbegin(); it != m.rend(); ++it) {
            result += it->first;
        }
        REQUIRE(result == "cba");
    }
}

struct NonCopyableValue {
    int value;
    NonCopyableValue(int v) : value(v) {}
    NonCopyableValue(const NonCopyableValue&) = delete;
    NonCopyableValue& operator=(const NonCopyableValue&) = delete;
    NonCopyableValue(NonCopyableValue&&) = default;
};

TEST_CASE("Map移动语义测试", "[map][move]") {
    std::map<std::string, NonCopyableValue> m;
    m.emplace("key", NonCopyableValue(42)); // 使用emplace避免拷贝
    REQUIRE(m["key"].value == 42);
}

TEST_CASE("Map值更新测试", "[map][update]") {
    std::map<std::string, int> m = {{"A", 100}, {"B", 200}};

    SECTION("使用[]更新") {
        m["A"] = 101;
        REQUIRE(m["A"] == 101);
    }

    SECTION("使用insert_or_assign更新") {
        m.insert_or_assign("B", 201);
        REQUIRE(m["B"] == 201);
    }

    SECTION("使用emplace_hint更新") {
        auto it = m.find("A");
        m.emplace_hint(it, "A", 102); // 提示位置，但键已存在，不更新
        REQUIRE(m["A"] == 100); // emplace_hint不覆盖已有值
    }
}