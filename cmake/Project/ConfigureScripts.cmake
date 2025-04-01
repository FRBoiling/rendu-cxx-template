#**********************************
#  Created by boil on 2025/02/21.
#**********************************

# 增强路径获取的健壮性
function(GetScriptsBasePath variable)
  set(_base_path "${CMAKE_SOURCE_DIR}/src/scripts")
  if(NOT IS_DIRECTORY "${_base_path}")
    message(FATAL_ERROR "脚本基础路径不存在: ${_base_path}")
  endif()
  set(${variable} "${_base_path}" PARENT_SCOPE)
endfunction()

# 存储脚本模块绝对路径
function(GetPathToScriptModule module variable)
  GetScriptsBasePath(SCRIPTS_BASE_PATH)
  set(${variable} "${SCRIPTS_BASE_PATH}/${module}" PARENT_SCOPE)
endfunction()

# 生成脚本模块项目名
function(GetProjectNameOfScriptModule module variable)
  string(TOLOWER "scripts_${module}" GENERATED_NAME)
  set(${variable} "${GENERATED_NAME}" PARENT_SCOPE)
endfunction()

# 优化模块列表获取性能
function(GetScriptModuleList variable)
  GetScriptsBasePath(BASE_PATH)
  file(GLOB _module_dirs LIST_DIRECTORIES true
      RELATIVE "${BASE_PATH}"
      "${BASE_PATH}/*")

  set(_valid_modules)
  foreach(_module IN LISTS _module_dirs)
    if(IS_DIRECTORY "${BASE_PATH}/${_module}")
      list(APPEND _valid_modules "${_module}")
    endif()
  endforeach()
  set(${variable} "${_valid_modules}" PARENT_SCOPE)
endfunction()

# 转换模块名为变量名
function(ScriptModuleNameToVariable module variable)
  string(TOUPPER "${module}" _upper_module)
  set(${variable} "SCRIPTS_${_upper_module}" PARENT_SCOPE)
endfunction()


