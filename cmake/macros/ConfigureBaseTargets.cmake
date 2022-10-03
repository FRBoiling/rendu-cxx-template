#**********************************
#  Created by boil on 2022/8/14.
#**********************************

# 一个接口库，使目标com对其他目标可用
add_library(rendu-compile-option-interface INTERFACE)

# Use -std=c++11 instead of -std=gnu++11
set(CXX_EXTENSIONS OFF)

# 一个接口库，使目标特性对其他目标可用
add_library(rendu-feature-interface INTERFACE)
target_compile_features(rendu-feature-interface
  INTERFACE
    cxx_std_17)

# 一个接口库，使警告级别对其他目标可用。这个接口目标是通过平台特定的脚本设置的
add_library(rendu-warning-interface INTERFACE)

# 用于所有其他接口的接口
add_library(rendu-default-interface INTERFACE)
target_link_libraries(rendu-default-interface
  INTERFACE
    rendu-compile-option-interface
    rendu-feature-interface)

# 一个用于屏蔽所有警告的接口
add_library(rendu-no-warning-interface INTERFACE)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(rendu-no-warning-interface
    INTERFACE
      /W0)
else()
  target_compile_options(rendu-no-warning-interface
    INTERFACE
      -w)
endif()

# 更改默认行为的接口库，自动隐藏符号。
add_library(rendu-hidden-symbols-interface INTERFACE)

# 接口合并，提供依赖项目标使用的标志和定义
add_library(rendu-dependency-interface INTERFACE)
target_link_libraries(rendu-dependency-interface
  INTERFACE
    rendu-default-interface
    rendu-no-warning-interface
    rendu-hidden-symbols-interface)

# 提供核心目标使用的标志和定义的接口合并
add_library(rendu-core-interface INTERFACE)
target_link_libraries(rendu-core-interface
  INTERFACE
    rendu-default-interface
    rendu-warning-interface)
