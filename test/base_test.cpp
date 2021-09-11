#include <iostream>
#include <utility>

#include "catch2/catch_all.hpp"
#include "conf-gen/generator.hpp"

// clang-format off
CONF_GROUP(
  test_conf_t, 
  Input(F64, thresh, 1.8, Show, "is open", "open as default", {.min = 1, .max = 3, .step = 0.2 }), 

);
// clang-format on

TEST_CASE("test_case", "[temp]") { CHECK(true); }