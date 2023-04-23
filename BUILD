load('@com_google_protobuf//:protobuf.bzl', 'cc_proto_library')

cc_proto_library(
    name = 'proto',
    srcs = glob(['proto/*.proto']),
    visibility = ["//visibility:public"],
)

cc_binary(
    name = 'main_work',
    srcs = glob(['src/cpp/*.cpp', 'src/cpp/*.h']),
    linkopts = ['-L/usr/local/lib', '-lcppkafka', '-lpthread'],
    copts = ['-g'],
    deps = [':proto']
)
