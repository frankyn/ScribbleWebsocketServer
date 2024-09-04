workspace(name = "com_github_frankyn_scribblewebsocketserver")


load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load(
    "@bazel_tools//tools/build_defs/repo:git.bzl",
    "git_repository",
)

http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip"],
  strip_prefix = "googletest-609281088cfefc76f9d0ce82e1ff6c30cc3591e5",
)

_RULES_BOOST_COMMIT = "652b21e35e4eeed5579e696da0facbe8dba52b1f"
http_archive(
    name = "com_github_nelhage_rules_boost",
    sha256 = "c1b8b2adc3b4201683cf94dda7eef3fc0f4f4c0ea5caa3ed3feffe07e1fb5b15",
    strip_prefix = "rules_boost-%s" % _RULES_BOOST_COMMIT,
    urls = [
        "https://github.com/nelhage/rules_boost/archive/%s.tar.gz" % _RULES_BOOST_COMMIT,
    ],
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()

http_archive(
    name = "rules_foreign_cc",
    sha256 = "1148c7f918b81bb51b05299f39ac8584aeb96f97bd1c6450ebb8d52202d7ea27",
    strip_prefix = "rules_foreign_cc-0.9.0a",
    url = "https://github.com/frankyn/rules_foreign_cc/archive/refs/tags/v0.9.0a.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://bazelbuild.github.io/rules_foreign_cc/0.9.0/flatten.html#rules_foreign_cc_dependencies
rules_foreign_cc_dependencies()

_ALL_CONTENT = """\
filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)
"""

http_archive(
    name = "mysql-connector-cpp",
    build_file_content = _ALL_CONTENT,
    sha256 = "ae2b03d8f74044dd016796f617958537528c565f1cc1f6298d9cf8b0631f086c",
    strip_prefix = "mysql-connector-cpp-8.0.30",
    url = "https://github.com/mysql/mysql-connector-cpp/archive/8.0.30.zip",
)


http_archive(
    name = "org_lua",
    build_file = "@com_github_frankyn_scribblewebsocketserver//bazel/third_party:lua.BUILD",
    sha256 = "2640fc56a795f29d28ef15e13c34a47e223960b0240e8cb0a82d9b0738695333",
    strip_prefix = "lua-5.1.5",
    urls = [
        "http://www.lua.org/ftp/lua-5.1.5.tar.gz",
    ],
)

git_repository(
       name = "bazel_clang_tidy",
       commit = "783aa523aafb4a6798a538c61e700b6ed27975a7",
       remote = "https://github.com/erenon/bazel_clang_tidy.git",
)