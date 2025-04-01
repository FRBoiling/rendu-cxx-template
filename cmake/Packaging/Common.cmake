# cmake/Packing/Common.cmake
# 打包通用配置
function(set_pack_common_config)
  include(InstallRequiredSystemLibraries)

  # 基础包信息
  set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
  set(CPACK_PACKAGE_VENDOR "Your Company")
  set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Modern C++ Application")
  set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
  set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

  # 跨平台压缩格式
  set(CPACK_SOURCE_GENERATOR "ZIP;TGZ")
  set(CPACK_SOURCE_IGNORE_FILES "/build/;/.git/;/.vscode/")

  # 组件安装配置
  set(CPACK_COMPONENT_RUNTIME_DISPLAY_NAME "Runtime Components")
  set(CPACK_COMPONENT_DEVELOPMENT_DISPLAY_NAME "Development Files")
  set(CPACK_COMPONENT_DOCUMENTATION_DISPLAY_NAME "Documentation")

  include(CPack)
endfunction()