load(
    "//bazel:envoy_build_system.bzl",
    "envoy_package",
)
load(
    "//test/extensions:extensions_build_system.bzl",
    "envoy_extension_cc_test",
)

licenses(["notice"])  # Apache 2

envoy_package()

envoy_extension_cc_test(
    name = "sni_cluster_test",
    srcs = ["sni_cluster_test.cc"],
    extension_names = ["envoy.filters.network.sni_cluster"],
    rbe_pool = "6gig",
    deps = [
        "//source/extensions/filters/network/sni_cluster",
        "//source/extensions/filters/network/sni_cluster:config",
        "//test/mocks/network:network_mocks",
        "//test/mocks/server:factory_context_mocks",
        "//test/mocks/stream_info:stream_info_mocks",
    ],
)
