load("@com_grail_bazel_compdb//:aspects.bzl", "compilation_database")

cc_library(
    name = "confgen",
    hdrs = [
        "include/conf-gen/def.hpp",
        "include/conf-gen/generator.hpp",
        "include/conf-gen/group.hpp",
        "include/conf-gen/items.hpp",
    ],
    includes = ["include"],
    visibility = ["//visibility:public"],
    deps = [
        "@com_github_nlohmann_json//:json",
    ],
)

compilation_database(
    name = "compdb",
    testonly = True,
    # Does not work with regular msvc compiler
    disable = select({
        "@bazel_tools//tools/cpp:msvc": True,
        "//conditions:default": False,
    }),
    targets = [
        ":confgen",
        "//test:all_catch2_cases",
    ],
    visibility = ["//visibility:private"],
)
