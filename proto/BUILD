package(default_visibility = ["//visibility:public"])

load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")

proto_library(
    name = "network_type_proto",
    srcs = ["network_type.proto"],
)

cc_proto_library(
    name = "network_type_cc_proto",
    deps = [":network_type_proto"],
)

proto_library(
    name = "signature_info_proto",
    srcs = ["signature_info.proto"],
)

cc_proto_library(
    name = "signature_info_cc_proto",
    deps = [":signature_info_proto"],
)

proto_library(
    name = "replica_info_proto",
    srcs = ["replica_info.proto"],
    deps = [":signature_info_proto"],
)

cc_proto_library(
    name = "replica_info_cc_proto",
    deps = [":replica_info_proto"],
)

proto_library(
    name = "resdb_proto",
    srcs = ["resdb.proto"],
    deps = [
        ":replica_info_proto",
        ":signature_info_proto",
    ],
)

cc_proto_library(
    name = "resdb_cc_proto",
    deps = [":resdb_proto"],
)

proto_library(
    name = "client_test_proto",
    srcs = ["client_test.proto"],
    deps = [
    ],
)

cc_proto_library(
    name = "client_test_cc_proto",
    deps = [":client_test_proto"],
)

proto_library(
    name = "kv_server_proto",
    srcs = ["kv_server.proto"],
    deps = [
    ],
)

cc_proto_library(
    name = "kv_server_cc_proto",
    deps = [":kv_server_proto"],
)

proto_library(
    name = "logging_proto",
    srcs = ["logging.proto"],
    deps = [
    ],
)

cc_proto_library(
    name = "logging_cc_proto",
    deps = [":logging_proto"],
)

proto_library(
    name = "checkpoint_info_proto",
    srcs = ["checkpoint_info.proto"],
    deps = [
    ],
)

cc_proto_library(
    name = "checkpoint_info_cc_proto",
    deps = [":checkpoint_info_proto"],
)

proto_library(
    name = "broadcast_proto",
    srcs = ["broadcast.proto"],
    deps = [
    ],
)

cc_proto_library(
    name = "broadcast_cc_proto",
    deps = [":broadcast_proto"],
)
