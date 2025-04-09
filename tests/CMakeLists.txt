#**********************************
#  Created by boil on 2022/10/24.
#**********************************

### ============================
### 测试套件配置
### ============================

if (RENDU_BUILD_TESTING)
    get_filename_component(FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    project(${FOLDER_NAME})

    include(CTest)  # 启用测试框架

    rendu_add_subdirectories(
        DIR    ${CMAKE_CURRENT_SOURCE_DIR}
        EXCLUDE_DIRS
    )

    # 恢复用户定义的源码构
    if (NOT WITH_SOURCE_TREE STREQUAL "hierarchical-folders")
        set_property(GLOBAL PROPERTY USE_FOLDERS OFF)  # 禁用自动文件夹分组
    endif ()
endif ()





