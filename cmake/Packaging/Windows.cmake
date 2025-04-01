# cmake/Packing/Windows.cmake
# Windows平台打包配置
function(set_windows_pack_config)
  if(NOT WIN32)
    return()
  endif()

  # NSIS配置
  set(CPACK_GENERATOR "NSIS;ZIP")
  set(CPACK_NSIS_MUI_ICON "${PROJECT_SOURCE_DIR}/assets/installer.ico")
  set(CPACK_NSIS_MUI_UNIICON "${PROJECT_SOURCE_DIR}/assets/uninstaller.ico")
  set(CPACK_NSIS_MODIFY_PATH ON)
  set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

  # MSI配置
  set(CPACK_WIX_UPGRADE_GUID "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX")
  set(CPACK_WIX_PRODUCT_ICON "${PROJECT_SOURCE_DIR}/assets/icon.ico")
endfunction()