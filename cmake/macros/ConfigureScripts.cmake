#**********************************
#  Created by boil on 2022/8/14.
#**********************************

# 返回源目录中脚本目录的基本路径
function(WarnAboutSpacesInBuildPath)
  # 只检查win32，因为unix不允许路径中有空格
  if(WIN32)
    string(FIND "${CMAKE_BINARY_DIR}" " " SPACE_INDEX_POS)

    if(SPACE_INDEX_POS GREATER -1)
      message("")
      # message(WARNING " *** WARNING!\n"
      #                 " *** Your selected build directory contains spaces!\n"
      #                 " *** Please note that this will cause issues!")
      message(WARNING " *** WARNING!\n"
                      " *** 您选择的生成目录包含空格!\n"
                      " *** 请注意，这将导致问题!")               
    endif()
  endif()
endfunction()

# 返回源目录中脚本目录的基本路径
function(GetScriptsBasePath variable)
  set(${variable} "${CMAKE_SOURCE_DIR}/src/server/scripts" PARENT_SCOPE)
endfunction()

# 在变量中存储给定模块的绝对路径
function(GetPathToScriptModule module variable)
  GetScriptsBasePath(SCRIPTS_BASE_PATH)
  set(${variable} "${SCRIPTS_BASE_PATH}/${module}" PARENT_SCOPE)
endfunction()

# 将给定模块的项目名称存储在变量中
function(GetProjectNameOfScriptModule module variable)
  string(TOLOWER "scripts_${SCRIPT_MODULE}" GENERATED_NAME)
  set(${variable} "${GENERATED_NAME}" PARENT_SCOPE)
endfunction()

# 创建所有脚本模块的列表，并将其存储在给定变量中。
function(GetScriptModuleList variable)
  GetScriptsBasePath(BASE_PATH)
  file(GLOB LOCALE_SCRIPT_MODULE_LIST RELATIVE
    ${BASE_PATH}
    ${BASE_PATH}/*)

  set(${variable})
  foreach(SCRIPT_MODULE ${LOCALE_SCRIPT_MODULE_LIST})
    GetPathToScriptModule(${SCRIPT_MODULE} SCRIPT_MODULE_PATH)
    if(IS_DIRECTORY ${SCRIPT_MODULE_PATH})
      list(APPEND ${variable} ${SCRIPT_MODULE})
    endif()
  endforeach()
  set(${variable} ${${variable}} PARENT_SCOPE)
endfunction()

# 将给定的脚本模块名转换为包含链接类型的变量名
function(ScriptModuleNameToVariable module variable)
  string(TOUPPER ${module} ${variable})
  set(${variable} "SCRIPTS_${${variable}}")
  set(${variable} ${${variable}} PARENT_SCOPE)
endfunction()

# 存储在给定变量中是否需要动态链接
function(IsDynamicLinkingRequired variable)
  if(SCRIPTS MATCHES "dynamic")
    set(IS_DEFAULT_VALUE_DYNAMIC ON)
  endif()

  GetScriptModuleList(SCRIPT_MODULE_LIST)
  set(IS_REQUIRED OFF)
  foreach(SCRIPT_MODULE ${SCRIPT_MODULE_LIST})
    ScriptModuleNameToVariable(${SCRIPT_MODULE} SCRIPT_MODULE_VARIABLE)
    if((${SCRIPT_MODULE_VARIABLE} STREQUAL "dynamic") OR
        (${SCRIPT_MODULE_VARIABLE} STREQUAL "default" AND IS_DEFAULT_VALUE_DYNAMIC))
      set(IS_REQUIRED ON)
      break()
    endif()
  endforeach()
  set(${variable} ${IS_REQUIRED} PARENT_SCOPE)
endfunction()

# 存储本机变量名
function(GetNativeSharedLibraryName module variable)
  if(WIN32)
    set(${variable} "${module}.dll" PARENT_SCOPE)
  elseif(APPLE)
    set(${variable} "lib${module}.dylib" PARENT_SCOPE)
  else()
    set(${variable} "lib${module}.so" PARENT_SCOPE)
  endif()
endfunction()

# 在变量中存储本机安装路径
function(GetInstallOffset variable)
  if(WIN32)
    set(${variable} "${CMAKE_INSTALL_PREFIX}/scripts" PARENT_SCOPE)
  else()
    set(${variable} "${CMAKE_INSTALL_PREFIX}/bin/scripts" PARENT_SCOPE)
  endif()
endfunction()
