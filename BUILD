load('@com_google_protobuf//:protobuf.bzl', 'cc_proto_library')

cc_proto_library(
    name = 'proto',
    srcs = glob(['proto/*.proto']),
    visibility = ["//visibility:public"],
)

cc_library(
    name = 'lru',
    srcs = glob(['src/cpp/lru/*.cpp']),
    hdrs = glob(['src/cpp/lru/*.h']),
    copts = ['-g'],
    visibility = ["//visibility:public"],
    deps = []
)

cc_library(
    name = 'service',
    srcs = glob(['src/cpp/service/*.cpp']),
    hdrs = glob(['src/cpp/service/*.h']),
    copts = ['-g'],
    visibility = ["//visibility:public"],
    deps = [':proto', '@brpc//:brpc', '@com_github_google_glog//:glog', '@com_github_nlohmann_json//:json', ':lru']
)

cc_binary(
    name = 'main_work',
    srcs = glob(['src/cpp/*.cpp']),
    linkopts = ['-lunwind', '-ltcmalloc', '-L/usr/local/lib', '-lcppkafka', '-lpthread'],
    copts = ['-g'],
    deps = [':proto', '@brpc//:brpc', '@com_github_google_glog//:glog', ':service']
)
