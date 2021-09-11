#include <iostream>

#include "catch2/catch_all.hpp"
#include "nlohmann/json.hpp"

struct item {
  int val;
  std::string name;

  NLOHMANN_DEFINE_TYPE_INTRUSIVE(item, val, name)
};

TEST_CASE("test_case", "[temp]") {
  CHECK(true);
  nlohmann::json j;

  std::vector<item> items{{.val = 1, .name = "123"}, {2, "567ga"}};

  j = items;

  struct empty_type {};

  j = empty_type();

  std::cout << j.size() << std::endl;
  std::cout << j.get<std::vector<item>>().at(0).name << std::endl;

  std::cout << nlohmann::json(items) << std::endl;
}