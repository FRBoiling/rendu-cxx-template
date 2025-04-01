# cmake/Project/ConfigureBaseTargets.cmake
# ====================================================

#**********************************
#  Created by boil on 2025/02/21.
#**********************************

include(ShowMessage)

print_section("项目Targets配置")

#======================= 核心编译选项接口 =======================#
# 定义全局编译选项接口库
add_library(rendu-compile-option-interface INTERFACE)
print_option("编译选项接口库" "已创建" ${Green})

# 注入构建类型标识符
target_compile_definitions(rendu-compile-option-interface
    INTERFACE
    _BUILD_DIRECTIVE="$<CONFIG>")
print_option("构建类型标识符" "已注入" ${Green})

#======================= 功能接口配置 =======================#
add_library(rendu-feature-interface INTERFACE)
add_library(rendu-warning-interface INTERFACE)  # 警告增强接口
add_library(rendu-no-warning-interface INTERFACE)  # 警告抑制接口

print_option("特性/警告增强/警告抑制接口 库" "已初始化" ${Green})

#======================= 默认接口聚合 =======================#
add_library(rendu-default-interface INTERFACE)
target_link_libraries(rendu-default-interface
    INTERFACE
    rendu-compile-option-interface
    rendu-feature-interface
    $<IF:$<BOOL:${WITH_WARNINGS}>,  # 动态选择警告模式
    rendu-warning-interface,
    rendu-no-warning-interface
    >
)

print_option("默认接口库（包含动态警告配置）" "已聚合" ${Green})

#======================= 依赖项接口聚合 =======================#
add_library(rendu-dependency-interface INTERFACE)
target_link_libraries(rendu-dependency-interface
    INTERFACE
    rendu-default-interface
    rendu-hidden-symbols-interface
    rendu-no-warning-interface  # 强制依赖项禁用警告
)

print_option("依赖项接口库（包含强制依赖项禁用警告）" "已聚合" ${Green})

#======================= 核心接口聚合 =======================#
add_library(rendu-core-interface INTERFACE)
target_link_libraries(rendu-core-interface
    INTERFACE
    rendu-default-interface
    rendu-warning-interface  # 核心模块强制启用警告
)

print_final_option("核心模块专用接口" "已聚合" ${Green})
