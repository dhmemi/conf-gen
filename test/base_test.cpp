#include <iostream>
#include <utility>

#include "catch2/catch_all.hpp"
#include "conf-gen/generator.hpp"

TEST_CASE("Check getter and setter", "[Check][getter][setter]") {

  CONF_GROUP(test_conf_t,
             Check(Show, Bool, open, false, "is open", "open as default"), );

  test_conf_t conf;
  CHECK_FALSE(conf.get_open());
  CHECK(conf.set_open(true));
  CHECK(conf.get_open());
}

TEST_CASE("Input getter and setter", "[Input][getter][setter]") {

  CONF_GROUP(test_conf_t,
             Input(Show, Int, thresh, 5, "threshold", "", {1, 9, 2}),
             Input(Hide, F64, rate, 0.5, "rate", "any info", {0, 1, 0.05}), );

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
  CONF_GROUP(
      test_conf_t,
      String(Show, Str, path, "12345", "path", "", {.regex = "^[0-9]*$"}), );

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
             Range(Show, VecInt, thresh, {10, 160}, "threshold", "", {.l0 = 0, .r0 =128, .l1 = 128, .r1 = 255}), 
             Range(Show, VecF64, rate, {0.2, 0.8}, "rate", "", {0., 1., 0., 1.}), );
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
             Select(Show, Int, kernal, 1, "kernal", "", {{1}, {100}, {999}}), );

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
             Array(Advan,
                   VecF64,
                   val_list,
                   {1.0, 1.5, 3.5},
                   "kernal",
                   "",
                   {.min = 0, .max = 10., .step = 0.1}), );
}