cc_test(
    name = "all_catch2_cases",
    srcs = [
        "test_all_types.cpp",
        "test_getter_setter.cpp",
        "test_group.cpp",
    ],
    visibility = ["//:__pkg__"],
    deps = [
        "//:confgen",
        "@com_github_catchorg_catch2//:catch2_main",
        "@com_github_nlohmann_json//:json",
    ],
)
