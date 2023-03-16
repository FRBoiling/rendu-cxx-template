#**********************************
#  Created by boil on 2022/8/14.
#**********************************

# 检查我们在什么平台上(64位或32位)，并创建一个比CMAKE_SIZEOF_VOID_P更简单的测试
if(CMAKE_SIZEOF_VOID_P MATCHES 8)
    set(PLATFORM 64)
    MESSAGE(STATUS "Detected 64-bit platform")
else()
    set(PLATFORM 32)
    MESSAGE(STATUS "Detected 32-bit platform")
endif()

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
  MESSAGE(STATUS "current platform: Linux ")
  include("${CMAKE_SOURCE_DIR}/cmake/platform/linux/settings.cmake")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
  MESSAGE(STATUS "current platform: Windows")
  include("${CMAKE_SOURCE_DIR}/cmake/platform/win/settings.cmake")
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  MESSAGE(STATUS "current platform: Mac OS X")
  include("${CMAKE_SOURCE_DIR}/cmake/platform/mac/settings.cmake")
ELSE ()
  MESSAGE(STATUS "other platform: ${CMAKE_SYSTEM_NAME}")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")
