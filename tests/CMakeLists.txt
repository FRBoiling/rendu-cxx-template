#**********************************
#  Created by boil on 2022/10/24.
#**********************************

### ============================
### 测试套件配置
### ============================

if (RENDU_BUILD_TESTING)
    include(CTest)  # 启用测试框架
    # Catch2 测试框架集成
    list(APPEND CMAKE_MODULE_PATH
            "${Catch2_SOURCE_DIR}/contrib")
    include(Catch)

    # 测试用例构建
    rendu_add_subdirectory(
            ${CMAKE_CURRENT_SOURCE_DIR}
            # Exclude
    )

    # 恢复用户定义的源码组织结构
    if (NOT WITH_SOURCE_TREE STREQUAL "hierarchical-folders")
        set_property(GLOBAL PROPERTY USE_FOLDERS OFF)  # 禁用自动文件夹分组
    endif ()
endif ()



