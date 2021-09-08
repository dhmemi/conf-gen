#include "catch2/catch_all.hpp"
#include "nlohmann/json.hpp"

TEST_CASE("test_case", "[temp]") {
  CHECK(true);
  nlohmann::json j;
  const std::string name = "123324";
  j[name] = "safdsfsdgf";
  CHECK(j[name] == "afdsfsdgf");
}