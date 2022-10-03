/*
* Created by boil on 2022/9/25.
*/

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#define GET_SLOTKEY(group,slot) ((group)*1000+(slot))
#define GET_GROUP_FROM_SLOTKEY(slot_key) ((slot_key)/1000)
#define GET_SLOT_FROM_SLOTKEY(slot_key) ((slot_key)%1000)

TEST_CASE("tests_main") {
  auto group = 1;
  auto slot = 12;
  auto slot_key = GET_SLOTKEY(group,slot);
  REQUIRE(slot_key);
  REQUIRE(GET_GROUP_FROM_SLOTKEY(slot_key) == group);
  REQUIRE(GET_SLOT_FROM_SLOTKEY(slot_key) == slot);

  std::set<int> test_set;
  test_set.insert(1);
  test_set.insert(2);
  test_set.insert(1);

  REQUIRE(test_set.size() == 2);
  test_set.erase(1);

  REQUIRE(test_set.size() == 2);

  
}