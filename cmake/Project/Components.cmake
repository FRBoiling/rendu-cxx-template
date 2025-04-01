# cmake/Project/Components.cmake
# ====================================================
# 项目组件管理系统
# 负责定义和管理所有项目组件及其依赖关系

# [变量] 注册的组件列表
set(RENDU_COMPONENTS "" CACHE INTERNAL "注册的项目组件列表")

# [变量] 组件依赖关系图
set(RENDU_COMPONENT_DEPENDENCIES "" CACHE INTERNAL "组件依赖关系图")

# [函数] 注册一个新组件
# 参数:
#   COMPONENT_NAME - 组件名称
#   DEPENDS        - 依赖的组件列表
#   ENABLE_BY_DEFAULT - 是否默认启用
function(rendu_register_component)
  set(options ENABLE_BY_DEFAULT)
  set(oneValueArgs COMPONENT_NAME)
  set(multiValueArgs DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # 添加组件到全局列表
  list(APPEND RENDU_COMPONENTS ${ARG_COMPONENT_NAME})
  set(RENDU_COMPONENTS "${RENDU_COMPONENTS}" CACHE INTERNAL "注册的项目组件列表")

  # 记录依赖关系
  set(_deps "")
  foreach(dep IN LISTS ARG_DEPENDS)
    list(APPEND _deps "${ARG_COMPONENT_NAME}->${dep}")
  endforeach()
  list(APPEND RENDU_COMPONENT_DEPENDENCIES ${_deps})
  set(RENDU_COMPONENT_DEPENDENCIES "${RENDU_COMPONENT_DEPENDENCIES}" CACHE INTERNAL "组件依赖关系图")

  # 设置组件选项
  option(RENDU_ENABLE_${ARG_COMPONENT_NAME} "启用 ${ARG_COMPONENT_NAME} 组件" ${ARG_ENABLE_BY_DEFAULT})
endfunction()

# [函数] 获取组件的所有依赖项
# 参数:
#   COMPONENT - 目标组件
#   OUTPUT_VAR - 输出变量名
function(rendu_get_component_dependencies COMPONENT OUTPUT_VAR)
  set(_result "")
  foreach(rel IN LISTS RENDU_COMPONENT_DEPENDENCIES)
    if(rel MATCHES "^${COMPONENT}->(.+)$")
      list(APPEND _result ${CMAKE_MATCH_1})
    endif()
  endforeach()
  set(${OUTPUT_VAR} ${_result} PARENT_SCOPE)
endfunction()

# [函数] 配置所有启用的组件
function(rendu_configure_components)
  foreach(comp IN LISTS RENDU_COMPONENTS)
    if(RENDU_ENABLE_${comp})
      message(STATUS "启用组件: ${comp}")

      # 处理依赖
      rendu_get_component_dependencies(${comp} deps)
      foreach(dep IN LISTS deps)
        if(NOT RENDU_ENABLE_${dep})
          message(FATAL_ERROR "组件 ${comp} 需要依赖 ${dep}，但该组件未启用")
        endif()
      endforeach()

      # 添加组件子目录
      add_subdirectory(${CMAKE_SOURCE_DIR}/libs/${comp})
    else()
      message(STATUS "禁用组件: ${comp}")
    endif()
  endforeach()
endfunction()