#**********************************
#  Created by boil on 2025/02/21.
#**********************************


include(ShowMessage)

print_section("平台配置")
# 平台配置文件
print_option("系统名称" "${CMAKE_SYSTEM_NAME}" ${Green})

#======================= 平台位数检测 =======================#
# 通过指针大小判断32/64位平台
if(CMAKE_SIZEOF_VOID_P MATCHES 8)
  set(PLATFORM 64)
  print_option("系统架构位数" "${PLATFORM}位 (指针大小: ${CMAKE_SIZEOF_VOID_P} bytes)" ${Green})
else()
  set(PLATFORM 32)
  print_option("系统架构位数" "${PLATFORM}位 (指针大小: ${CMAKE_SIZEOF_VOID_P} bytes)" ${Green})
endif()

#======================= 平台配置加载 =======================#
if (WIN32)
  include("${CMAKE_SOURCE_DIR}/cmake/Platform/Windows/settings.cmake")
  print_final_option("Windows平台配置" "✔ 已加载" ${Cyan})
elseif (APPLE)  # 新增macOS专用分支
  include("${CMAKE_SOURCE_DIR}/cmake/Platform/MacOS/settings.cmake")
  print_final_option("Apple平台配置" "✔ 已加载" ${Cyan})
elseif (UNIX)   # 剩余Unix系统默认为Linux
  include("${CMAKE_SOURCE_DIR}/cmake/Platform/Linux/settings.cmake")
  print_final_option("Linux平台配置" "✔ 已加载" ${Cyan})
endif ()
message("")
