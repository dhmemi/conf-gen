#include <utility>
#include <vector>

#include "catch2/catch_all.hpp"
#include "conf-gen/generator.hpp"

enum color_t { Red, Green, Blue, Gray, White, Black };

// clang-format off
CONF_GROUP(BoxSize,
    Check(Bool, enable, true, Show, "enable size"),
    Input(Int, width, 10, Show, "width", {10, 100, 10}, "The box width, should be multiple of 10 in [10, 100]"),
    Input(Int, height, .value = 50, .ctrl = {10, 200, 20}, .comment = "box's height"),
    Input(F64, scale, .value = 0.5, .name = "size scale", .ctrl = {0.2, 1, 0.05}));

CONF_GROUP(Color,
    Enums(color_t, fg_color, Green, Show, "foreground color", {{Red, "Red"}, {Green, "Green"}, {Blue, "Blue"}}),
    Enums(color_t, bg_color, .value = Black, .ctrl = {{.value = Gray, .name = "Gray"}, {White, "White"}, {Black, "Black"}}),
    Refer(F64, ref_scale, Input, Show, "size scale", {.path = "box_size.scale"}));
  
CONF_GROUP(Box,
    Group(BoxSize, box_size, {}, Show, "size"),
    Group(Color, colors, {}, Advan, "colors"),
    String(Str, text, "Hello World", Fixed), 
    Range(VecF64, rotate_range, {0., 10.}, Advan, "rorate range", {.l0 = 0., .r0 = 90., .l1 = 0., .r1 = 90.}),
    Array(VecInt, kernals, {3}, Hide, "kernal sizes", {1, 9, 2}, "kernal size"));

// clang-format on

TEST_CASE("An example for all meta type and data types.", "all") {
  Box conf;
  CHECK_NOTHROW(conf.to_json());
}