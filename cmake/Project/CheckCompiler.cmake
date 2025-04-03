#**********************************
#  Created by boil on 2025/02/21.
#**********************************

print_section("编译器配置")

# 输出编译器配置摘要
print_option("编译器ID" "${CMAKE_CXX_COMPILER_ID}" ${Green})
print_option("编译器前端类型" "${CMAKE_CXX_COMPILER_FRONTEND_VARIANT}" ${Green})

#======================= 编译器配置加载 =======================#
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" OR CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
  include("${CMAKE_SOURCE_DIR}/cmake/compiler/msvc/settings.cmake")
  print_final_option("MSVC编译器配置" "✔ 已加载" ${Cyan})
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  include("${CMAKE_SOURCE_DIR}/cmake/compiler/clang/settings.cmake")
  print_final_option("Clang编译器配置" "✔ 已加载" ${Cyan})
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  include("${CMAKE_SOURCE_DIR}/cmake/compiler/gcc/settings.cmake")
  print_final_option("GCC编译器配置" "✔ 已加载" ${Cyan})
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  include("${CMAKE_SOURCE_DIR}/cmake/compiler/icc/settings.cmake")
  print_final_option("Intel编译器配置" "✔ 已加载" ${Cyan})
endif ()

