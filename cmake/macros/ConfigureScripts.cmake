# ConfigureScripts.cmake
# =========================
# 脚本模块相关工具函数
# =========================

# 检查构建路径中是否包含空格（仅 Windows 需关注）
function(rendu_warn_about_spaces_in_build_path)
  if(WIN32)
    string(FIND "${CMAKE_BINARY_DIR}" " " space_index_pos)
    if(space_index_pos GREATER -1)
      message("")
      message(WARNING " *** WARNING!\n"
                      " *** Your selected build directory contains spaces!\n"
                      " *** Please note that this will cause issues!")
    endif()
  endif()
endfunction()

# 获取脚本目录的基础路径
function(rendu_get_scripts_base_path variable)
  set(${variable} "${CMAKE_SOURCE_DIR}/src/server/scripts" PARENT_SCOPE)
endfunction()

# 获取指定模块的绝对路径
function(rendu_get_path_to_script_module module variable)
  rendu_get_scripts_base_path(scripts_base_path)
  set(${variable} "${scripts_base_path}/${module}" PARENT_SCOPE)
endfunction()

# 获取指定模块的项目名
function(rendu_get_project_name_of_script_module module variable)
  string(TOLOWER "scripts_${module}" generated_name)
  set(${variable} "${generated_name}" PARENT_SCOPE)
endfunction()

# 获取所有脚本模块列表
function(rendu_get_script_module_list variable)
  rendu_get_scripts_base_path(base_path)
  file(GLOB locale_script_module_list RELATIVE
    ${base_path}
    ${base_path}/*)

  set(${variable} "")
  foreach(script_module ${locale_script_module_list})
    rendu_get_path_to_script_module(${script_module} script_module_path)
    if(IS_DIRECTORY ${script_module_path})
      list(APPEND ${variable} ${script_module})
    endif()
  endforeach()
  set(${variable} ${${variable}} PARENT_SCOPE)
endfunction()

# 将脚本模块名转换为其对应的变量名
function(rendu_script_module_name_to_variable module variable)
  string(TOUPPER ${module} upper_module)
  set(var_name "RENDU_SCRIPTS_${upper_module}")
  set(${variable} ${var_name} PARENT_SCOPE)
endfunction()

# 判断是否需要动态链接
function(rendu_is_dynamic_linking_required variable)
  if(RENDU_SCRIPTS MATCHES "dynamic")
    set(is_default_value_dynamic ON)
  endif()

  rendu_get_script_module_list(script_module_list)
  set(is_required OFF)
  foreach(script_module ${script_module_list})
    rendu_script_module_name_to_variable(${script_module} script_module_variable)
    if((${${script_module_variable}} STREQUAL "dynamic") OR
        (${${script_module_variable}} STREQUAL "default" AND is_default_value_dynamic))
      set(is_required ON)
      break()
    endif()
  endforeach()
  set(${variable} ${is_required} PARENT_SCOPE)
endfunction()

# 获取平台原生共享库文件名
function(rendu_get_native_shared_library_name module variable)
  if(WIN32)
    set(${variable} "${module}.dll" PARENT_SCOPE)
  elseif(APPLE)
    set(${variable} "lib${module}.dylib" PARENT_SCOPE)
  else()
    set(${variable} "lib${module}.so" PARENT_SCOPE)
  endif()
endfunction()

# 获取脚本安装路径
function(rendu_get_install_offset variable)
  if(WIN32)
    set(${variable} "${CMAKE_INSTALL_PREFIX}/scripts" PARENT_SCOPE)
  else()
    set(${variable} "${CMAKE_INSTALL_PREFIX}/bin/scripts" PARENT_SCOPE)
  endif()
endfunction()