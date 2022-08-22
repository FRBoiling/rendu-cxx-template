if(CMAKE_VERSION VERSION_LESS "3.16.0")
  if(MSVC)
    #指定预编译头内存分配的最大限制,修复了使用PCH时的编译器问题- MSVC2012中的/Ym标志被编译器调整.
    #因此，我们需要用/Zm设置一个上限，以避免差异
    #注意:这个工作区在MSVC 2017上也被验证是必需的
    set(COTIRE_PCH_MEMORY_SCALING_FACTOR 500)
  endif()

  include(cotire)

  function(ADD_CXX_PCH TARGET_NAME_LIST PCH_HEADER)
    # 对每个目标使用标头
    foreach(TARGET_NAME ${TARGET_NAME_LIST})
      # 禁用unity版本
      set_target_properties(${TARGET_NAME} PROPERTIES COTIRE_ADD_UNITY_BUILD OFF)
      # 设置前缀头
      set_target_properties(${TARGET_NAME} PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT ${PCH_HEADER})
      # 代码错误的解决方法: https://github.com/sakra/cotire/issues/138
      set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)
    endforeach()

    cotire(${TARGET_NAME_LIST})
  endfunction(ADD_CXX_PCH)
else()
  function(ADD_CXX_PCH TARGET_NAME_LIST PCH_HEADER)
    foreach(TARGET_NAME ${TARGET_NAME_LIST})
      target_precompile_headers(${TARGET_NAME} PRIVATE ${PCH_HEADER})
    endforeach()
  endfunction(ADD_CXX_PCH)
endif()
