cc_test(
    name = "rfc_6455_unit_tests",
    srcs = ["rfc_6455/unit_protocol.cpp"],
    deps = [":rfc_6455"],
)

cc_library(
    name = "rfc_6455",
    visibility = ["//visibility:public"],
    srcs = ["rfc_6455/RFC_6455.cpp",],
    hdrs = ["WSProtocol.h", "rfc_6455/RFC_6455.h"],
    deps = [
        "@boost//:uuid",
        "@boost//:algorithm",
        "@boost//:archive",
    ],
)