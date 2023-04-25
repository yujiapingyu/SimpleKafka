workspace(name = "simple_kafka")

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

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = ["https://ghproxy.com/https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
)

http_archive(
    name = "brpc",
    urls = [
         "https://ghproxy.com/https://github.com/apache/incubator-brpc/archive/refs/tags/0.9.7.tar.gz"
    ],
    sha256 = "71088f673c197a6b760a20ac6b07f481cc4a0b08917f35acbf325de01b3f3fe7",
    strip_prefix = "brpc-0.9.7",
)

http_archive(
    name = "com_github_google_glog",
    sha256 = "122fb6b712808ef43fbf80f75c52a21c9760683dae470154f02bddfc61135022",
    strip_prefix = "glog-0.6.0",
    urls = ["https://ghproxy.com/https://github.com/google/glog/archive/v0.6.0.zip"],
)

http_archive(
    name = "com_github_nlohmann_json",
    build_file = "//third_party/json:BUILD",
    sha256 = "1155fd1a83049767360e9a120c43c578145db3204d2b309eba49fbbedd0f4ed3",
    strip_prefix = "json-3.10.4",
    urls = [
        "https://github.com/nlohmann/json/archive/refs/tags/v3.10.4.tar.gz",
    ],
)
