//
// Created by boil on 2023/3/10.
//

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

void MergeContainer(std::map<int, std::set<int>> &container1, std::map<int, std::set<int>> &container3) {
  for (auto it = container1.begin(); it != container1.end(); ++it) {
    container3[it->first].insert(it->second.begin(), it->second.end());
  }
}

class TestSortClass {
 public:
  TestSortClass() : m_key(0), m_value(0) {}
 public:
  int m_key;
  int m_value;
 public:
  bool operator == (const TestSortClass& ps) const
  {
      return this->m_value == ps.m_value;
  }

  bool operator < (const TestSortClass& ps) const
  {
    return this->m_value < ps.m_value;
  }
};


template <typename T>
void removeRepeat(std::vector<T>& vec) {
  std::sort(vec.begin(), vec.end());
  vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
};

int main() {

  std::map<int, std::set<int> > container1;
  std::map<int, std::set<int> > container2;
  std::map<int, std::set<int> > container3;
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 3; ++j) {
      container1[i].insert(j);
      container2[i].insert(j + 3);
    }
  }
  MergeContainer(container1, container3);
  MergeContainer(container2, container3);

  std::map<int, TestSortClass> temp_back_groups;
  int key = 10;
  int value = 1;
  while (key > 0) {
    TestSortClass item;
    item.m_value = value;
    item.m_key = key;
    temp_back_groups[item.m_key] = item;
    key--;
    value++;
  }

  std::vector<TestSortClass> temp_vec;
  for (auto it = temp_back_groups.begin(); it != temp_back_groups.end(); ++it) {
    temp_vec.push_back(it->second);
  }
  std::sort(temp_vec.begin(), temp_vec.end(),
            [](const TestSortClass &l, const TestSortClass &r) {
              return l.m_value < r.m_value;
            });

  std::vector<int> vec(0);
  for (int i = 0; i < 10; i++) {
    vec.push_back(i);
  }
  for (int i = 2; i < 5; i++) {
    vec.push_back(i);
  }

  removeRepeat(vec);


  std::vector<TestSortClass> vec1;
  for (int i = 1; i < 10; i++) {
    TestSortClass item;
    item.m_value = i;
    item.m_key = i;
    vec1.push_back(item);
  }
  for (int i = 2; i < 5; i++) {
    TestSortClass item;
    item.m_value = i;
    item.m_key = i;
    vec1.push_back(item);
  }
  std::sort(vec1.begin(), vec1.end(),
            [](const TestSortClass& l, const TestSortClass& r){
    return l.m_value < r.m_value;
  });
  vec1.erase(std::unique(vec1.begin(), vec1.end()), vec1.end());

  std::set<TestSortClass> set1;
  for (int i = 1; i < 10; i++) {
    TestSortClass item;
    item.m_value = i;
    item.m_key = i;
    set1.insert(item);
  }
  for (int i = 2; i < 5; i++) {
    TestSortClass item;
    item.m_value = i;
    item.m_key = i;
    set1.insert(item);
  }

  std::cout << "Hello container" << std::endl;

}
