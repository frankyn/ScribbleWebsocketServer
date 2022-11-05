cc_test(
  name = "websocket_unit_tests",
  srcs = ["rfc_6455/unit_protocol.cpp"],
  deps = ["@com_google_googletest//:gtest_main", ":websocket"],
)

cc_library(
    name = "websocket",
    visibility = ["//visibility:public"],
    srcs = ["rfc_6455/WebSocket.cpp",],
    hdrs = ["rfc_6455/WebSocket.h"],
    deps = [
        "@boost//:uuid",
        "@boost//:algorithm",
        "@boost//:archive",
        "@boost//:json",
    ],
)