
# 跨平台安装路径处理
function(GetInstallOffset variable)
  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(_install_prefix "/opt/rendu")
  else()
    set(_install_prefix "${CMAKE_INSTALL_PREFIX}")
  endif()

  if(WIN32)
    set(_offset "${_install_prefix}/scripts")
  else()
    set(_offset "${_install_prefix}/bin/scripts")
  endif()
  set(${variable} "${_offset}" PARENT_SCOPE)
endfunction()