
#include <iostream>
#include <sstream>

#include "catch2/catch_all.hpp"
#include "conf-gen/generator.hpp"

TEST_CASE("Group function test", "[Group]") {

  enum color_t { Red, Green, Blue, Gray, White, Black };

  // clang-format off
  CONF_GROUP(BoxSize,
             Input(Int, width, 10, Show, "width", {10, 100, 10}, "The box width, should be multiple of 10 in [10, 100]"),
             Input(Int, height, .value = 50, .ctrl = {10, 200, 20}));

  CONF_GROUP(Color,
             Enums(color_t, fg_color, Green, Show, "foreground color", {{Red, "Red"}, {Green, "Green"}, {Blue, "Blue"}}),
             Enums(color_t, bg_color, .value = Black, .ctrl = {{.value = Gray, .name = "Gray"}, {White, "White"}, {Black, "Black"}}));
  
  CONF_GROUP(Box,
            Group(BoxSize, box_size, {}, Show, "size"),
            Group(Color, colors, {}, Advan, "colors"),
            String(Str, text, "Hello World", Fixed));

  // clang-format on

  Box conf;

  SECTION("conf can be convert to json and output to stream") {
    std::ostringstream oss;
    CHECK_NOTHROW(oss << conf.to_json());
  }

  SECTION("read conf from json") {
    nlohmann::json outer_json = R"({
        "box_size": {
            "value": {
                "height": {
                    "value": 110
                }
            }
        },
        "colors": {
            "value": {
                "bg_color": {
                    "value": 4
                },
                "fg_color": {
                    "value": 1
                }
            }
        },
        "text": {
            "value": "item changed outside, but will not be read into as it's Fixed."
        }
    })"_json;

    outer_json.get_to(conf);

    CHECK(conf.get_box_size().get_height() == 110);
    CHECK(conf.get_colors().get_bg_color() == White);
    CHECK(conf.get_text() == "Hello World");
  }

  SECTION("we can get item infomation through group.at() or get value info "
          "through group.value_at()") {
    CHECK(conf.at("text").meta_type() == confgen::String);
    CHECK(conf.value_at<Color>("colors").at("bg_color").data_type() == "Int");
    CHECK(conf.value_at<Color>("box_size").value_at<int>("width") == 10);
    CHECK(conf.at("box_size").data_type() == "Group");
  }
}
