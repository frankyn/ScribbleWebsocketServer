cc_binary(
    name = "scribble_runner",
    srcs = ["main.cpp", "Channel.cpp", "ChannelSelector.cpp", "Connection.cpp", "ConnectionsWaiting.cpp", "ListenerClass.cpp", "Channel.h", "ChannelSelector.h", "Connection.h", "ConnectionsWaiting.h", "ListenerClass.h"],
    deps = ["//src/common:common", "//src/protocols:rfc_6455", "//src/mysql:mysql"],
)

cc_test(
    name = "unit_scriptloader",
    srcs = ["unit_scriptloader.cc"],
    deps = [":scriptloader"],
)

cc_library(
    name = "scriptloader",
    srcs = ["ScriptLoader.cc"],
    hdrs = ["ScriptLoader.h"],
    deps = ["@org_lua//:lua", "//src/common:common"],
)