# -*-Python-*-

cc_library(
    name = "yatc_cli_lib",
    defines = ["CLI_ONLY=1", "LOGIN_ONLY=1", "HAVE_TOMMATH=1", "BAZEL_BUILD=1"],
    srcs = [
        "creatureui.cpp", "//:creatureui.h",
        "thingui.cpp", "//:thingui.h",
        "effectui.cpp", "//:effectui.h",
        "objects.cpp", "//:objects.h",
        "distanceui.cpp", "//:distanceui.h",
        "itemui.cpp", "//:itemui.h",
        "notifications.cpp", "//:notifications.h",
        
        "//gamecontent:inventory.cpp",
        "//gamecontent:globalvars.cpp",
        "//gamecontent:creature.cpp",
        "//gamecontent:map.cpp",
        "//gamecontent:container.cpp",

        "//:stdinttypes.h",
        "//:sprite.h",
        "//:defines.h",

        "//gamecontent:creature.h",
        "//gamecontent:thing.h",
        "//gamecontent:enums.h",
        "//gamecontent:position.h",
        "//gamecontent:globalvars.h",
        "//gamecontent:container.h",
        "//gamecontent:itemcontainer.h",
        "//gamecontent:shop.h",
        "//gamecontent:inventory.h",
        "//gamecontent:map.h",
        "//gamecontent:item.h",
        "//gamecontent:viplist.h",

        "//net:encryption.cpp",
        "//net:networkmessage.cpp",
        "//net:connection.cpp",

        "//net:connection.h",
        "//net:networkmessage.h",
        "//net:encryption.h",
        "//net:protocolconfig.h",
        "//net:protocolconfig.cpp",
        "//net:protocolgame.h",
        "//net:protocollogin.h",
        "//net:protocollogin.cpp",
        "//net:rsa.h",
        "//net:rsa.cpp",

        "//:util.cpp",
        "//:util.h",
        "//:product.h",
        "//:macutil.h",

        "//:debugprint.cpp",

        ":protocolgamehdrs",
        "yatc_cli.cpp",
    ],
    copts = ["-isystem external/tommath"],
    deps = [
        "//:debugprint",
        "//:fassert",
        
        "//net:enum_hdr",
        "@tommath//:tommath",
        #"//:yatc_lib", # We could pull in far less stuff. See Makefile.
    ] + select({
        "//:darwin": [
            "@libsdl12//:sdlmain",
            "//:macclipboard",
            "//:macutil",
        ],
        "//conditions:default": [],
    }),
    data = [
        "@tibia854//:Tibia.dat",
        "@tibia854//:Tibia.pic",
        "@tibia854//:Tibia.spr",
    ],
)
cc_binary(
    name = "yatc_cli",
    deps = [
        ":yatc_cli_lib",
    ],
)

filegroup(
    name = "protocolgamehdrs",
    srcs = [
        "//net:protocolgame74.h",
        "//net:protocolgame76.h",
        "//net:protocolgame78.h",
        "//net:protocolgame80.h",
        "//net:protocolgame81.h",
        "//net:protocolgame811.h",
        "//net:protocolgame82.h",
        "//net:protocolgame821.h",
        "//net:protocolgame822.h",
        "//net:protocolgame83.h",
        "//net:protocolgame831.h",
        "//net:protocolgame84.h",
        "//net:protocolgame841.h",
        "//net:protocolgame842.h",
        "//net:protocolgame85.h",
        "//net:protocolgame853.h",
        "//net:protocolgame854.h",
    ],
)
filegroup(
    name = "protocolgamesrcs",
    srcs = [
        "//net:protocolgame.cpp",
        "//net:protocolgame74.cpp",
        "//net:protocolgame76.cpp",
        "//net:protocolgame78.cpp",
        "//net:protocolgame80.cpp",
        "//net:protocolgame81.cpp",
        "//net:protocolgame811.cpp",
        "//net:protocolgame82.cpp",
        "//net:protocolgame821.cpp",
        "//net:protocolgame822.cpp",
        "//net:protocolgame83.cpp",
        "//net:protocolgame831.cpp",
        "//net:protocolgame84.cpp",
        "//net:protocolgame841.cpp",
        "//net:protocolgame842.cpp",
        "//net:protocolgame85.cpp",
        "//net:protocolgame853.cpp",
        "//net:protocolgame854.cpp",
    ],
)
