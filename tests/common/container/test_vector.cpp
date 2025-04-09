/*
* Created by boil on 2022/10/2.
*/

#include "catch/rd_catch2.hpp"
#include <vector>

TEST_CASE("Vector基本操作", "[vector][basic]") {
    std::vector<int> v;

    SECTION("默认构造") {
        REQUIRE(v.empty());
        REQUIRE(v.size() == 0);
    }

    SECTION("初始化列表构造") {
        v = {1, 3, 5};
        REQUIRE(v.size() == 3);
        REQUIRE(v[0] == 1);
        REQUIRE(v[2] == 5);
    }

    SECTION("填充构造") {
        std::vector<double> v(5, 3.14);
        REQUIRE(v.size() == 5);
        REQUIRE(v[3] == Catch::Approx(3.14));
    }

    SECTION("越界访问") {
        REQUIRE_THROWS_AS(v.at(0), std::out_of_range); // at()检查边界
        // REQUIRE_NOTHROW(v[0]); // []不检查边界，可能导致未定义行为
    }
}

TEST_CASE("Vector元素修改", "[vector][modify]") {
    std::vector<std::string> v;

    SECTION("尾部插入") {
        v.push_back("apple");
        v.emplace_back("banana");
        REQUIRE(v.size() == 2);
        REQUIRE(v.back() == "banana");
    }

    SECTION("指定位置插入") {
        v = {"a", "c"};
        v.insert(v.begin() + 1, "b"); // 在位置1插入"b"
        REQUIRE(v[1] == "b");
    }

    SECTION("删除元素") {
        v = {"x", "y", "z"};
        v.pop_back(); // 删除尾部
        REQUIRE(v.size() == 2);
        REQUIRE(v.back() == "y");

        v.erase(v.begin()); // 删除头部
        REQUIRE(v.front() == "y");
    }

    SECTION("清空向量") {
        v = { "one", "two" };
        v.clear();
        REQUIRE(v.empty());
    }
}

TEST_CASE("Vector容量管理", "[vector][capacity]") {
    std::vector<int> v;

    SECTION("容量增长") {
        v.reserve(10); // 预分配空间
        REQUIRE(v.capacity() >= 10);
        REQUIRE(v.size() == 0);

        for (int i = 0; i < 5; ++i)
            v.push_back(i);
        REQUIRE(v.capacity() >= 10); // 容量不缩减
    }

    SECTION("缩容操作") {
        v = {1, 2, 3, 4, 5};
        v.resize(3); // 调整大小为3
        REQUIRE(v.size() == 3);
        REQUIRE(v.capacity() >= 5); // 容量可能不变

        v.shrink_to_fit(); // 尝试释放多余内存
        REQUIRE(v.capacity() >= v.size()); // 标准仅要求容量>=size
    }
}

TEST_CASE("Vector迭代器", "[vector][iterator]") {
    std::vector<char> v = {'a', 'b', 'c', 'd'};

    SECTION("正向遍历") {
        std::string result;
        for (auto it = v.begin(); it != v.end(); ++it)
            result += *it;
        REQUIRE(result == "abcd");
    }

    SECTION("反向遍历") {
        std::string result;
        for (auto it = v.rbegin(); it != v.rend(); ++it)
            result += *it;
        REQUIRE(result == "dcba");
    }

    SECTION("迭代器修改") {
        for (auto& c : v)
            c = std::toupper(c);
        REQUIRE(v[0] == 'A');
    }
}

TEST_CASE("Vector元素访问", "[vector][access]") {
    std::vector<int> v = {10, 20, 30};

    REQUIRE(v.front() == 10);       // 第一个元素
    REQUIRE(v.back() == 30);        // 最后一个元素
    REQUIRE(v.data()[1] == 20);     // 原始数据指针

    // 引用修改
    v.front() = 100;
    REQUIRE(v[0] == 100);
}

struct ThrowingCopy {
    int value;
    explicit ThrowingCopy(int v) : value(v) {}
    ThrowingCopy(const ThrowingCopy& other) : value(other.value) {
        if (other.value == 42) throw std::runtime_error("Copy error");
    }
};

TEST_CASE("Vector异常安全", "[vector][exception]") {
    std::vector<ThrowingCopy> v;
    v.emplace_back(10);

    SECTION("插入异常") {
        REQUIRE_THROWS(v.push_back(ThrowingCopy(42)));
        REQUIRE(v.size() == 1); // 插入失败，向量状态不变
    }

    SECTION("移动安全") {
        std::vector<std::unique_ptr<int>> v;
        v.push_back(std::make_unique<int>(1));
        v.push_back(std::make_unique<int>(2));

        v.pop_back(); // 安全释放资源
        REQUIRE(v.size() == 1);
    }
}

TEST_CASE("Vector比较操作", "[vector][comparison]") {
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {1, 2, 3};
    std::vector<int> c = {1, 2, 3, 4};

    REQUIRE(a == b);    // 元素相同
    REQUIRE(a != c);    // 大小不同
    REQUIRE(a < c);     // 按字典序比较
}

TEST_CASE("Vector性能测试", "[vector][performance]") {
    BENCHMARK("尾部插入1000元素") {
        std::vector<int> v;
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        return v.size();
    };

    BENCHMARK("预分配后尾部插入1000元素") {
        std::vector<int> v;
        v.reserve(1000);
        for (int i = 0; i < 1000; ++i)
            v.push_back(i);
        return v.size();
    };
}