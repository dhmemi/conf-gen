workspace(name = "dhmemi_confgen")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_github_nlohmann_json",
    build_file = "//third_party:nlohmann_json.BUILD",
    sha256 = "7d0edf65f2ac7390af5e5a0b323b31202a6c11d744a74b588dc30f5a8c9865ba",
    strip_prefix = "json-3.8.0",
    urls = ["https://github.com/nlohmann/json/archive/v3.8.0.tar.gz"],
)

# for clangd auto complation
http_archive(
    name = "com_grail_bazel_compdb",
    sha256 = "efd853bea81a09ed97c6e1a6c6bbf8d83aab9b548c88b63cf63049851c182d57",
    strip_prefix = "bazel-compilation-database-master",
    urls = ["https://github.com/dhmemi/bazel-compilation-database/archive/refs/heads/master.zip"],
)

load("@com_grail_bazel_compdb//:config.bzl", "config_clang_compdb")

config_clang_compdb(
    filter_flags = ["-fno-canonical-system-headers"],
    workspace_name = "dhmemi_confgen",
)

http_archive(
    name = "com_github_catchorg_catch2",
    sha256 = "d81bb2732485074649087ca4f3ceb80fed6900dad2885115baa0ba1fd465b0d3",
    strip_prefix = "Catch2-3.0.0-preview3",
    urls = [
        "https://github.com/catchorg/Catch2/archive/refs/tags/v3.0.0-preview3.zip",
    ],
)
