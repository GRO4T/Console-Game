load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "rules_foreign_cc",
    sha256 = "476303bd0f1b04cc311fc258f1708a5f6ef82d3091e53fd1977fa20383425a6a",
    strip_prefix = "rules_foreign_cc-0.10.1",
    url = "https://github.com/bazelbuild/rules_foreign_cc/releases/download/0.10.1/rules_foreign_cc-0.10.1.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://bazelbuild.github.io/rules_foreign_cc/0.10.1/flatten.html#rules_foreign_cc_dependencies
rules_foreign_cc_dependencies()

_ALL_CONTENT = """\
filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)
"""

# allegro source code repository
# http_archive(
#     name = "allegro",
#     build_file_content = _ALL_CONTENT,
#     strip_prefix = "allegro-5.2.9.1",
#     urls = [
#         "https://github.com/liballeg/allegro5/releases/download/5.2.9.1/allegro-5.2.9.1.tar.gz"
#     ],
#     # sha256 = "0b8e7465dc5e98c757cc3650a20a7843ee4c3edf50aaf60bb33fd879690d2c73",
# )

# http_archive(
#     name = "sdl",
#     build_file_content = _ALL_CONTENT,
#     strip_prefix = "SDL2-2.30.0",
#     urls = [
#         "https://github.com/libsdl-org/SDL/releases/download/release-2.30.0/SDL2-2.30.0.tar.gz"
#     ],
#     # sha256 = "0b8e7465dc5e98c757cc3650a20a7843ee4c3edf50aaf60bb33fd879690d2c73",
# )

# http_archive(
#     name = "gainput",
#     build_file_content = _ALL_CONTENT,
#     strip_prefix = "gainput-1.0.0",
#     urls = [
#         "https://github.com/jkuhlmann/gainput/archive/refs/tags/v1.0.0.tar.gz"
#     ],
#     # sha256 = "0b8e7465dc5e98c757cc3650a20a7843ee4c3edf50aaf60bb33fd879690d2c73",
# )

http_archive(
    name = "sfml",
    build_file_content = _ALL_CONTENT,
    strip_prefix = "SFML-2.6.1",
    urls = [
        "https://github.com/SFML/SFML/archive/refs/tags/2.6.1.zip"
    ],
    # sha256 = "0b8e7465dc5e98c757cc3650a20a7843ee4c3edf50aaf60bb33fd879690d2c73",
)

