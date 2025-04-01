# cmake/Packing/Components.cmake
# 定义打包组件和安装规则
function(define_pack_components)
  # 安装可执行文件
  install(TARGETS server
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      COMPONENT runtime
  )

  # 安装静态库
  install(TARGETS core network
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      COMPONENT development
  )

  # 安装头文件
  install(DIRECTORY ${PROJECT_SOURCE_DIR}/libs/core/include/
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/core
      COMPONENT development
  )
  install(DIRECTORY ${PROJECT_SOURCE_DIR}/libs/network/include/
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/network
      COMPONENT development
  )

  # 安装文档
  install(DIRECTORY ${PROJECT_SOURCE_DIR}/docs/
      DESTINATION ${CMAKE_INSTALL_DOCDIR}
      COMPONENT documentation
  )

  # 定义组件分组
  set(CPACK_COMPONENTS_ALL runtime development documentation)
endfunction()