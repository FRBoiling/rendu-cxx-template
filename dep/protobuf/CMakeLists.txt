#**********************************
#  Created by boil on 2022/8/14.
#**********************************

set(protobuf_source_dir ${CMAKE_CURRENT_SOURCE_DIR})
# Find revision ID and hash of the sourcetree
include(libprotobuf.cmake)
# set default buildoptions and print them
include(libprotobuf-lite.cmake)
# print out the results before continuing
include(libprotoc.cmake)

#set(protoc_files
#    ${protobuf_source_dir}/src/google/protobuf/compiler/main.cc
#    )
#
#add_executable(protoc ${protoc_files})
#target_link_libraries(protoc dep::protoc dep::protobuf)
#add_executable(protobuf::protoc ALIAS protoc)
