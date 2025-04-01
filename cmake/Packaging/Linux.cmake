# cmake/Packing/Linux.cmake
# Linux平台打包配置
function(set_linux_pack_config)
  if(NOT UNIX OR APPLE)
    return()
  endif()

  # DEB配置
  set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Maintainer <maintainer@example.com>")
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.27)")

  # RPM配置
  set(CPACK_RPM_PACKAGE_RELEASE 1)
  set(CPACK_RPM_PACKAGE_LICENSE "MIT")
  set(CPACK_RPM_PACKAGE_REQUIRES "glibc >= 2.27")

  set(CPACK_GENERATOR "DEB;RPM;TGZ")
endfunction()