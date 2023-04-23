workspace(name = "brpc_test")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# protobuf(Dependency: bazel_skylib, zlib)
http_archive(
    name = "bazel_skylib",
    sha256 = "2c62d8cd4ab1e65c08647eb4afe38f51591f43f7f0885e7769832fa137633dcb",
    strip_prefix = "bazel-skylib-0.7.0",
    urls = ["https://ghproxy.com/https://github.com/bazelbuild/bazel-skylib/archive/0.7.0.tar.gz"],
)

http_archive(
    name = "zlib_archive",
    build_file = "//third_party/zlib:BUILD",
    sha256 = "",
    strip_prefix = "zlib-1.2.13",
    urls = [
         "https://zlib.net/zlib-1.2.13.tar.gz",
    ],
)

bind(
    name = "zlib",
    actual = "@zlib_archive//:zlib",
)

http_archive(
    name = "com_google_protobuf",
    sha256 = "1c020fafc84acd235ec81c6aac22d73f23e85a700871466052ff231d69c1b17a",
    strip_prefix = "protobuf-5902e759108d14ee8e6b0b07653dac2f4e70ac73",
    urls = ["https://github.com/protocolbuffers/protobuf/archive/5902e759108d14ee8e6b0b07653dac2f4e70ac73.tar.gz"],
)

# brpc(Dependency: zlib, protobuf, gflags)
http_archive(
    name = "com_github_google_leveldb",
    build_file = "@brpc//:leveldb.BUILD",
    strip_prefix = "leveldb-a53934a3ae1244679f812d998a4f16f2c7f309a6",
    url = "https://ghproxy.com/https://github.com/google/leveldb/archive/a53934a3ae1244679f812d998a4f16f2c7f309a6.tar.gz"
)

git_repository(
    name   = "com_github_gflags_gflags",
    commit = "30dbc81fb5ffdc98ea9b14b1918bfe4e8779b26e", # v2.2.0 + fix of include path
    remote = "https://ghproxy.com/https://github.com/gflags/gflags.git"
)

http_archive(
    name = "brpc",
    urls = [
         "https://ghproxy.com/https://github.com/apache/incubator-brpc/archive/refs/tags/0.9.7.tar.gz"
    ],
    sha256 = "71088f673c197a6b760a20ac6b07f481cc4a0b08917f35acbf325de01b3f3fe7",
    strip_prefix = "brpc-0.9.7",
)