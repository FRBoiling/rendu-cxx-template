
# 增强动态链接检测
function(IsDynamicLinkingRequired variable)
  set(_required OFF)
  if(SCRIPTS MATCHES "dynamic")
    set(_default_dynamic ON)
  endif()

  GetScriptModuleList(_modules)
  foreach(_module IN LISTS _modules)
    ScriptModuleNameToVariable("${_module}" _var)
    if(("${${_var}}" STREQUAL "dynamic") OR
    ("${${_var}}" STREQUAL "default" AND _default_dynamic))
      set(_required ON)
      break()
    endif()
  endforeach()
  set(${variable} ${_required} PARENT_SCOPE)
endfunction()



# 获取平台动态库名称
function(GetNativeSharedLibraryName module variable)
  if(WIN32)
    set(${variable} "${module}.dll" PARENT_SCOPE)
  elseif(APPLE)
    set(${variable} "lib${module}.dylib" PARENT_SCOPE)
  else()
    set(${variable} "lib${module}.so" PARENT_SCOPE)
  endif()
endfunction()