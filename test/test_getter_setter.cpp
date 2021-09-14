/**
 * Copyright dhmemi.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http: //www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <utility>
#include <vector>

#include "catch2/catch_all.hpp"
#include "conf-gen/generator.hpp"

TEST_CASE("Check getter and setter", "[Check][getter][setter]") {

  CONF_GROUP(test_conf_t,
             Check(Bool, open, false, Show, "is open", "open as default"));

  test_conf_t conf;
  CHECK_FALSE(conf.get_open());
  CHECK(conf.set_open(true));
  CHECK(conf.get_open());
}

TEST_CASE("Input getter and setter", "[Input][getter][setter]") {
  // clang-format off
  CONF_GROUP(test_conf_t,
             Input(Int, thresh, 5, Show, "threshold", {.ge = 1, .le = 9, .step = 2}),
             Input(F64, rate, 0.5, Hide, "rate", {0, 1, 0.05}, "this is some annotation."));
  // clang-format on

  test_conf_t conf;
  CHECK(conf.get_thresh() == 5);

  SECTION("int Input type only support value which "
          "(value == (min + n*step) && value <= max).") {
    // NOLINTNEXTLINE(google-build-using-namespace)
    auto new_val = GENERATE(1, 3, 5, 7, 9);
    CHECK(conf.set_thresh(new_val));
    CHECK(conf.get_thresh() == new_val);
  }

  SECTION("set invalid value will return false and get the value will not "
          "change.") {
    auto origin_val = conf.get_thresh();
    // NOLINTNEXTLINE(google-build-using-namespace)
    auto invalid_val = GENERATE(0, 2, 4, 6, 8, 10, 100);
    CHECK_FALSE(conf.set_thresh(invalid_val));
    CHECK(conf.get_thresh() == origin_val);
  }

  SECTION("float or double Input type only support value which "
          "(value == (min + n*step) && value <= max).") {
    auto new_val =
        // NOLINTNEXTLINE(google-build-using-namespace)
        GENERATE(0., 0.05, 0.1, 0.3, 0.35, 0.45, 0.5, 0.7, 0.75, 0.95, 1.);
    CHECK(conf.set_rate(new_val));
    CHECK(conf.get_rate() == new_val);
  }

  SECTION("float or double Input type：set invalid value will return false and "
          "get the value will not change.") {
    auto origin_val = conf.get_rate();
    auto invalid_val =
        // NOLINTNEXTLINE(google-build-using-namespace)
        GENERATE(-0.1, 0.04, 0.21, 0.5005, 0.500005, 0.99999, 1.1, 100.);
    CHECK_FALSE(conf.set_rate(invalid_val));
    CHECK(conf.get_rate() == origin_val);
  }
}

TEST_CASE("String getter and setter", "[String][getter][setter]") {

  const std::string default_str = "12345";
  CONF_GROUP(test_conf_t,
             String(Str, path, "12345", Show, "path", {.regex = "^[0-9]*$"}));

  test_conf_t conf;
  CHECK(conf.get_path() == default_str);
  CHECK_FALSE(conf.set_path("a123234"));
  CHECK_FALSE(conf.set_path("1232c_34"));
  CHECK_FALSE(conf.set_path("1232.34"));
  CHECK(conf.set_path("09870987"));
  CHECK(conf.get_path() == "09870987");
  CHECK(conf.set_path("876232424352346213646745673241243345465765678"));
  CHECK(conf.get_path() == "876232424352346213646745673241243345465765678");
}

TEST_CASE("Range getter and setter", "[Range][getter][setter]") {
  // clang-format off
  CONF_GROUP(test_conf_t,
             Range(VecInt, thresh, {10, 160}, Show, "threshold", {.l0 = 0, .r0 =128, .l1 = 128, .r1 = 255}), 
             Range(VecF64, rate, {0.2, 0.8}, Show, "rate", {0., 1., 0., 1.}));
  // clang-format on

  using vec_int_t = std::vector<int>;
  using vec_f64_t = std::vector<double>;

  test_conf_t conf;
  SECTION("default value as the set") {
    CHECK(conf.get_thresh() == std::vector<int>{10, 160});
    CHECK(conf.get_rate() == std::vector<double>{0.2, 0.8});
  }

  SECTION("int range: set value out of range will return false") {
    vec_int_t origin_val = conf.get_thresh();
    auto invalid_val =
        // NOLINTNEXTLINE(google-build-using-namespace)
        GENERATE(vec_int_t{0, 127},
                 vec_int_t{129, 130},
                 vec_int_t{10, 256},
                 vec_int_t{-1, 200});

    CHECK_FALSE(conf.set_thresh(invalid_val));
    CHECK(conf.get_thresh() == origin_val);
  }

  SECTION("int range: set valid value will change the range") {
    auto new_min = GENERATE(take(5, random(0, 128)));   // NOLINT
    auto new_max = GENERATE(take(5, random(128, 255))); // NOLINT
    CHECK(conf.set_thresh({new_min, new_max}));
    CHECK(conf.get_thresh() == vec_int_t{new_min, new_max});
  }

  SECTION("float range: set value out of range will return false") {
    vec_f64_t origin_val = conf.get_rate();
    auto invalid_val =
        // NOLINTNEXTLINE(google-build-using-namespace)
        GENERATE(vec_f64_t{0, 1.001}, vec_f64_t{-1., 0.3}, vec_f64_t{0.8, 2.8});

    CHECK_FALSE(conf.set_rate(invalid_val));
    CHECK(conf.get_rate() == origin_val);
  }

  SECTION("float range: set valid value will change the range") {
    auto new_min = GENERATE(take(5, random(0., 1.))); // NOLINT
    auto new_max = GENERATE(take(5, random(0., 1.))); // NOLINT
    CHECK(conf.set_rate({new_min, new_max}));
    CHECK(conf.get_rate() == vec_f64_t{new_min, new_max});
  }
}

TEST_CASE("Select getter and setter") {
  CONF_GROUP(test_conf_t,
             Select(Int,
                    kernal,
                    .value = 1,
                    .permission = Show,
                    .name = "kernal",
                    .ctrl = {{1}, {100}, {999}},
                    .comment = "any comment, maybe a long string."));

  test_conf_t conf;

  SECTION("selects only support value in select item list.") {
    CHECK_FALSE(conf.set_kernal(2));
    CHECK_FALSE(conf.set_kernal(1000));
    CHECK_FALSE(conf.set_kernal(101));
    CHECK(conf.set_kernal(100));
    CHECK(conf.set_kernal(999));
    CHECK(conf.set_kernal(1));
  }
}

TEST_CASE("Array getter and setter") {
  CONF_GROUP(test_conf_t,
             Array(VecF64,
                   f64_list,
                   .value = {1.0, 1.5, 3.5},
                   .permission = Advan,
                   .name = "kernal",
                   .ctrl = {.ge = 0., .le = 10., .step = 0.1}),
             Array(VecStr,
                   str_list,
                   .value = {"abcderf"},
                   .permission = Show,
                   .name = "str list",
                   .ctrl = {.regex = "^[A-Za-z]+$"}));

  test_conf_t conf;

  using vec_f64_t = std::vector<double>;
  using vec_str_t = std::vector<std::string>;

  SECTION("double array: invalid values will be refused.") {
    auto origin_val = conf.get_f64_list();
    auto invalid_val =
        // NOLINTNEXTLINE(google-build-using-namespace)
        GENERATE(vec_f64_t{0, 1.2, 1.03},
                 vec_f64_t{0, 10.2, 1.3, 0.8},
                 vec_f64_t{-0.1, 1.2, 0.9, 2.4});
    CHECK_FALSE(conf.set_f64_list(invalid_val));
    CHECK(conf.get_f64_list() == origin_val);
  }

  SECTION("double array: valid values will be accepted.") {
    auto valid_val =
        // NOLINTNEXTLINE(google-build-using-namespace)
        GENERATE(vec_f64_t{0, 1.2, 1.3},
                 vec_f64_t{0, 10., 1.3, 0.8},
                 vec_f64_t{0.1, 1.2, 0.9, 2.4});
    CHECK(conf.set_f64_list(valid_val));
    CHECK(conf.get_f64_list() == valid_val);
  }

  SECTION("string array: invalid values will be refused.") {
    auto origin_val = conf.get_str_list();
    auto invalid_val =
        // NOLINTNEXTLINE(google-build-using-namespace)
        GENERATE(vec_str_t{"qweerefsdvsd.", "qwe"},
                 vec_str_t{"12VSBzdasvv"},
                 vec_str_t{"VSBz dasvv", "abcdefGH"},
                 vec_str_t{"tyuiolKMN", "cfdvbMM", "(asddfdf)"});
    CHECK_FALSE(conf.set_str_list(invalid_val));
    CHECK(conf.get_str_list() == origin_val);
  }

  SECTION("string array: valid values will be accepted.") {
    auto valid_val =
        // NOLINTNEXTLINE(google-build-using-namespace)
        GENERATE(vec_str_t{"qweerefsdvsdp", "qwe"},
                 vec_str_t{"AAVSBzdasvv"},
                 vec_str_t{"VSBzOdasvv", "abcdefGH"},
                 vec_str_t{"tyuiolKMN", "cfdvbMM", "asddfdf"});
    CHECK(conf.set_str_list(valid_val));
    CHECK(conf.get_str_list() == valid_val);
  }
}

TEST_CASE("Group getter and setter") {
  // clang-format off
  CONF_GROUP(test_group_t,
             Check(Bool, enable, false, Show, "is open", "open as default"),
             Input(Int, thresh, 5, Show, "threshold", {.ge = 1, .le = 9, .step = 2}),
             Input(F64, rate, 0.5, Hide, "rate", {0, 1, 0.05}, "this is some annotation."));

  CONF_GROUP(test_conf_t,  
             Range(VecF64, range, {0.2, 0.8}, Show, "rate", {0., 1., 0., 1.}),
             Group(test_group_t, inner_group, {}, Show));
  // clang-format on
  CONF_GROUP(test_conf_t2,
             Group(test_conf_t, other, {}, Show),
             Refer(Int,
                   refer_thresh,
                   Input,
                   Show,
                   "refer threshold",
                   {.path = "other.inner_group.thresh"},
                   "annotation of refer."));

  test_conf_t conf;

  SECTION("get value by chain") {
    CHECK(conf.get_inner_group().get_thresh() == 5);
    CHECK(conf.get_inner_group().get_rate() == 0.5);
    CHECK(conf.get_inner_group().get_enable() == false);
    CHECK(conf.get_range() == std::vector<double>{0.2, 0.8});
  }

  SECTION("set value by chain") {
    CHECK(conf.get_inner_group().set_thresh(7));
    CHECK_FALSE(conf.get_inner_group().set_thresh(8));
    CHECK(conf.get_inner_group().get_thresh() == 7);

    CHECK(conf.get_inner_group().set_rate(0.7));
    CHECK_FALSE(conf.get_inner_group().set_rate(0.74));
    CHECK(conf.get_inner_group().get_rate() == 0.7);
  }
}

TEST_CASE("Refer getter and setter") {
  // clang-format off
  CONF_GROUP(test_group_t,
             Check(Bool, enable, false, Show, "is open", "open as default"),
             Input(Int, thresh, 5, Show, "threshold", {.ge = 1, .le = 9, .step = 2}),
             Input(F64, rate, 0.5, Hide, "rate", {0, 1, 0.05}, "this is some annotation."));

  CONF_GROUP(test_conf_t,  
             Range(VecF64, range, {0.2, 0.8}, Show, "rate", {0., 1., 0., 1.}),
             Group(test_group_t, inner_group, {}, Show),
             Refer(F64, ref_rate, Input, Show, "ref_rate", {.path = ".inner_group.rate"}));

  CONF_GROUP(test_refer_t,
             Group(test_conf_t, other, {}, Show),
             Refer(Int, ref_thresh, Input, Show, "threshold", {.path = "other.inner_group.thresh"}, "annotation of refer."),
             Refer(test_group_t, ref_group, Group, Show, "group", {.path = "other.inner_group"}));
  // clang-format on

  test_refer_t conf;

  SECTION("Calling the getter of the referenced object is equivalent "
          "to calling the getter of the referenced object.") {
    // INFO(conf.get_ref_thresh());
    // INFO(conf.get_other().get_inner_group().get_thresh());
    CHECK(conf.get_ref_thresh() ==
          conf.get_other().get_inner_group().get_thresh());

    CHECK(conf.get_other().get_ref_rate() ==
          conf.get_other().get_inner_group().get_rate());
  }

  SECTION("int input: Calling the setter of the reference is equivalent to "
          "calling the setter of the referenced item.",
          "[int]") {
    auto new_val = GENERATE(1, 3, 5, 7, 9); // NOLINT

    CHECK(conf.set_ref_thresh(new_val));
    CHECK(conf.get_other().get_inner_group().get_thresh() == new_val);
  }

  SECTION("double input: Calling the setter of the reference is equivalent to "
          "calling the setter of the referenced item.",
          "[float]") {
    auto new_val = GENERATE(0., 0.1, 0.25, 0.55, 0.6, 0.95, 1.); // NOLINT

    CHECK(conf.get_other().set_ref_rate(new_val));
    CHECK(conf.get_other().get_inner_group().get_rate() == new_val);
  }

  SECTION("The reference follow the control rules of the referenced item.") {
    auto new_val = GENERATE(0., 0.35, 0.36, 0.75, 0.9999, 1.5, 3.0); // NOLINT

    CHECK(conf.get_other().set_ref_rate(new_val) ==
          conf.get_other().get_inner_group().set_rate(new_val));
  }
}