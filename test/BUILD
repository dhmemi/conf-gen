cc_test(
    name = "all_catch2_cases",
    srcs = [
        "base_test.cpp",
    ],
    visibility = ["//:__pkg__"],
    deps = [
        "//:confgen",
        "@com_github_catchorg_catch2//:catch2_main",
        "@com_github_nlohmann_json//:json",
    ],
)
