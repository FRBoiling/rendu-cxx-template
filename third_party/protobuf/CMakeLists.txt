set(target-name protobuf)

message(STATUS "[lib ] " ${target-name})
include(FetchContent)
FetchContent_Declare(
    ${target-name}
    URL https://github.com/protocolbuffers/protobuf/releases/download/v21.12/protobuf-all-21.12.tar.gz
    #    GIT_REPOSITORY https://gitee.com/lovelinpeach/protobuf.git
    #    GIT_TAG v21.12
    #    URL https://gitee.com/lovelinpeach/protobuf/repository/archive/v21.12
    FIND_PACKAGE_ARGS NAMES ${target-name}
    DOWNLOAD_EXTRACT_TIMESTAMP true
)
#set(protobuf_USE_EXTERNAL_GTEST 1)
# This will try calling find_package() first for both dependencies
FetchContent_MakeAvailable(${target-name})