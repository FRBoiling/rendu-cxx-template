# cmake/Packing/MacOS.cmake
# macOS平台打包配置
function(set_macos_pack_config)
  if(NOT APPLE)
    return()
  endif()

  # Bundle配置
  set(CPACK_GENERATOR "Bundle;TGZ")
  set(CPACK_BUNDLE_NAME "${PROJECT_NAME}")
  set(CPACK_BUNDLE_ICON "${PROJECT_SOURCE_DIR}/assets/mac_icon.icns")
  set(CPACK_BUNDLE_PLIST "${PROJECT_SOURCE_DIR}/cmake/Packing/Info.plist")

  # DMG配置
  set(CPACK_DMG_BACKGROUND_IMAGE "${PROJECT_SOURCE_DIR}/assets/dmg_background.png")
  set(CPACK_DMG_FORMAT "UDBZ")
endfunction()