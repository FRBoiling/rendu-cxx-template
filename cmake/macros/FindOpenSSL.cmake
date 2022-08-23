#**********************************
#  Created by boil on 2022/8/14.
#**********************************

#[=======================================================================[.rst:
FindOpenSSL
-----------

Find the OpenSSL encryption library.

可选组件
^^^^^^^^^^^^^^^^^^^
这个模块支持两个可选组件:``Crypto``和``SSL``。
这两个组件已经关联了导入的目标，如下所述。

导入的目标
^^^^^^^^^^^^^^^^
这个模块定义了以下内容: prop_tgt:`IMPORTED` targets:

``OpenSSL::SSL``
  The OpenSSL ``ssl`` library, if found.
``OpenSSL::Crypto``
  The OpenSSL ``crypto`` library, if found.

``OpenSSL: applink``
  OpenSSL ``applink``组件可能需要编译成MSVC下的项目。
  只有当发现OpenSSL版本不低于0.9.8时，此目标才可用。
  通过链接这个目标，上面的OpenSSL目标可以被链接，即使项目与上面的OpenSSL目标有不同的MSVC运行时配置。
  这个目标对MSVC以外的平台没有影响。
  除非你确实知道自己在做什么，否则最好将``OpenSSL::applink``target '链接为' PRIVATE '，并确保该目标在任何库或可执行文件的整个依赖图中最多链接一次:
``代码块:cmake

  target_link_libraries (myTarget私人OpenSSL:: applink)

  否则，在构建和链接时，您可能会遇到意想不到的随机问题，因为ISO C和ISO c++标准几乎没有说明链接过程应该是什么。

结果变量
^^^^^^^^^^^^^^^^
这个模块将在你的项目中设置以下变量:

``OPENSSL_FOUND``
  系统中已存在OpenSSL库。如果没有请求组件，则只需要加密库。
``OPENSSL_INCLUDE_DIR``
  OpenSSL include目录。
``OPENSSL_CRYPTO_LIBRARY``
  The OpenSSL crypto 库。
``OPENSSL_CRYPTO_LIBRARIES``
  The OpenSSL crypto 库及其依赖项。
``OPENSSL_SSL_LIBRARY``
  The OpenSSL SSL 库。
``OPENSSL_SSL_LIBRARIES``
  The OpenSSL SSL 库及其依赖项。
``OPENSSL_LIBRARIES``
  All OpenSSL 库及其依赖项。
``OPENSSL_VERSION``
  This is set to ``$major.$minor.$revision$patch`` (e.g. ``0.9.8s``).
``OPENSSL_APPLINK_SOURCE``
  上面提到的目标“OpenSSL::applink”中的源文件。如果发现openssl版本小于0.9.8或平台不是MSVC，则此变量将始终未定义。

提示
^^^^^^^^^^^^^^^^
设置``OPENSSL_ROOT_DIR``为OpenSSL安装的根目录。
设置``OPENSSL_USE_STATIC_LIBS``为``TRUE``来查找静态库。
设置``OPENSSL_MSVC_STATIC_RT``设置``TRUE``选择lib的MT版本。

#]=======================================================================]

set(OPENSSL_EXPECTED_VERSION "1.0")

macro(_OpenSSL_test_and_find_dependencies ssl_library crypto_library)
  if((CMAKE_SYSTEM_NAME STREQUAL "Linux") AND
     (("${ssl_library}" MATCHES "\\${CMAKE_STATIC_LIBRARY_SUFFIX}$") OR
      ("${crypto_library}" MATCHES "\\${CMAKE_STATIC_LIBRARY_SUFFIX}$")))
    set(_OpenSSL_has_dependencies TRUE)
    find_package(Threads)
  else()
    set(_OpenSSL_has_dependencies FALSE)
  endif()
endmacro()

function(_OpenSSL_add_dependencies libraries_var)
  if(CMAKE_THREAD_LIBS_INIT)
    list(APPEND ${libraries_var} ${CMAKE_THREAD_LIBS_INIT})
  endif()
  list(APPEND ${libraries_var} ${CMAKE_DL_LIBS})
  set(${libraries_var} ${${libraries_var}} PARENT_SCOPE)
endfunction()

function(_OpenSSL_target_add_dependencies target)
  if(_OpenSSL_has_dependencies)
    set_property( TARGET ${target} APPEND PROPERTY INTERFACE_LINK_LIBRARIES Threads::Threads )
    set_property( TARGET ${target} APPEND PROPERTY INTERFACE_LINK_LIBRARIES ${CMAKE_DL_LIBS} )
  endif()
endfunction()

if (UNIX)
  find_package(PkgConfig QUIET)
  pkg_check_modules(_OPENSSL QUIET openssl)
endif ()

# 通过调整CMAKE_FIND_LIBRARY_SUFFIXS支持静态库首选项
if(OPENSSL_USE_STATIC_LIBS)
  set(_openssl_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
  if(WIN32)
    set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .a ${CMAKE_FIND_LIBRARY_SUFFIXES})
  else()
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a )
  endif()
endif()

if (WIN32)
  # http://www.slproweb.com/products/Win32OpenSSL.html
  set(_OPENSSL_MSI_INSTALL_GUID "")
  if(PLATFORM EQUAL 64)
    set(_OPENSSL_MSI_INSTALL_GUID "117551DB-A110-4BBD-BB05-CFE0BCB3ED31")
    set(_OPENSSL_ROOT_HINTS
      ${OPENSSL_ROOT_DIR}
      ENV OPENSSL_ROOT_DIR
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\OpenSSL (64-bit)_is1;Inno Setup: App Path]"
      )
    file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" _programfiles)
    set(_OPENSSL_ROOT_PATHS
      "${_programfiles}/OpenSSL"
      "${_programfiles}/OpenSSL-Win64"
      "C:/OpenSSL/"
      "C:/OpenSSL-Win64/"
      )
  else()
    set(_OPENSSL_MSI_INSTALL_GUID "A1EEC576-43B9-4E75-9E02-03DA542D2A38")
    set(_OPENSSL_ROOT_HINTS
      ${OPENSSL_ROOT_DIR}
      ENV OPENSSL_ROOT_DIR
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\OpenSSL (32-bit)_is1;Inno Setup: App Path]"
      )
    file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" _programfiles)
    set(_OPENSSL_ROOT_PATHS
      "${_programfiles}/OpenSSL"
      "${_programfiles}/OpenSSL-Win32"
      "C:/OpenSSL/"
      "C:/OpenSSL-Win32/"
      )
  endif()
  unset(_programfiles)
  # 如果OpenSSL是使用.msi包而不是.exe安装的，Inno安装注册表值不会写入到Uninstall\OpenSSL，但因为它只是Inno安装周围的一个shim，它会写入卸载程序的位置，我们可以使用它来确定路径
  get_filename_component(_OPENSSL_MSI_INSTALL_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Inno Setup MSIs\\${_OPENSSL_MSI_INSTALL_GUID};]" DIRECTORY)
  if(NOT _OPENSSL_MSI_INSTALL_PATH STREQUAL "/")
    list(INSERT _OPENSSL_ROOT_HINTS 0 ${_OPENSSL_MSI_INSTALL_PATH})
  endif()
else ()
  set(_OPENSSL_ROOT_HINTS
    ${OPENSSL_ROOT_DIR}
    ENV OPENSSL_ROOT_DIR
    )
endif ()

if(HOMEBREW_PREFIX)
  list(APPEND _OPENSSL_ROOT_HINTS
    "${HOMEBREW_PREFIX}/opt/openssl@1.1"
    "${HOMEBREW_PREFIX}/opt/openssl@3")
endif()

set(_OPENSSL_ROOT_HINTS_AND_PATHS
    HINTS ${_OPENSSL_ROOT_HINTS}
    PATHS ${_OPENSSL_ROOT_PATHS}
    )

find_path(OPENSSL_INCLUDE_DIR
  NAMES
    openssl/ssl.h
  ${_OPENSSL_ROOT_HINTS_AND_PATHS}
  HINTS
    ${_OPENSSL_INCLUDEDIR}
    ${_OPENSSL_INCLUDE_DIRS}
  PATH_SUFFIXES
    include
)

if(WIN32 AND NOT CYGWIN)
  if(MSVC)
    # /MD和/MDd是标准值-如果有人想使用其他的，libname也必须改变
    # 这里使用ssl和ssleay32在调试中作为openssl < 0.9.8b的回调
    # 启用OPENSSL_MSVC_STATIC_RT来获得libs构建/MT(多线程无dll)在Visual c++命名规范中，这四种Windows库都有它的标准后缀:
    #   * MD for dynamic-release
    #   * MDd for dynamic-debug
    #   * MT for static-release
    #   * MTd for static-debug

    # 实现细节:
    # 我们使用位于VC子目录下的库，而不是父目录，即使:
    # libeay32MD.Lib与../libeay32.lib相同。
    # ssleay32MD.Lib与../ssleay32.lib相同
    # 启用OPENSSL_USE_STATIC_LIBS来使用位于lib/VC/static中的静态库

    if (OPENSSL_MSVC_STATIC_RT)
      set(_OPENSSL_MSVC_RT_MODE "MT")
    else ()
      set(_OPENSSL_MSVC_RT_MODE "MD")
    endif ()

    # 从OpenSSL 1.1开始，lib名称类似于libcrypto32MTd.lib 和 libssl32MTd.lib
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
        set(_OPENSSL_MSVC_ARCH_SUFFIX "64")
    else()
        set(_OPENSSL_MSVC_ARCH_SUFFIX "32")
    endif()

    if(OPENSSL_USE_STATIC_LIBS)
      set(_OPENSSL_STATIC_SUFFIX
        "_static"
      )
      set(_OPENSSL_PATH_SUFFIXES
        "lib/VC/static"
        "VC/static"
        "lib"
        )
    else()
      set(_OPENSSL_STATIC_SUFFIX
        ""
      )
      set(_OPENSSL_PATH_SUFFIXES
        "lib/VC"
        "VC"
        "lib"
        )
    endif ()

    find_library(LIB_EAY_DEBUG
      NAMES      
        # 当使用默认选项构建OpenSSL时，静态库名称以“_static”作为后缀。
        # 查看"libcrypto_static.lib"的优先级高于"libcrypto.dll"的导入库"libcrypto.lib"。
        libcrypto${_OPENSSL_STATIC_SUFFIX}${_OPENSSL_MSVC_ARCH_SUFFIX}${_OPENSSL_MSVC_RT_MODE}d
        libcrypto${_OPENSSL_STATIC_SUFFIX}${_OPENSSL_MSVC_RT_MODE}d
        libcrypto${_OPENSSL_STATIC_SUFFIX}d
        libeay32${_OPENSSL_STATIC_SUFFIX}${_OPENSSL_MSVC_RT_MODE}d
        libeay32${_OPENSSL_STATIC_SUFFIX}d
        crypto${_OPENSSL_STATIC_SUFFIX}d
        # 当使用“-static”选项构建OpenSSL时，只生成静态构建，并且不添加“_static”后缀。
        libcrypto${_OPENSSL_MSVC_ARCH_SUFFIX}${_OPENSSL_MSVC_RT_MODE}d
        libcrypto${_OPENSSL_MSVC_RT_MODE}d
        libcryptod
        libeay32${_OPENSSL_MSVC_RT_MODE}d
        libeay32d
        cryptod
      NAMES_PER_DIR
      ${_OPENSSL_ROOT_HINTS_AND_PATHS}
      PATH_SUFFIXES
        ${_OPENSSL_PATH_SUFFIXES}
    )

    find_library(LIB_EAY_RELEASE
      NAMES
        # 当使用默认选项构建OpenSSL时，静态库名称以“_static”作为后缀。
        # 查看"libcrypto_static.lib"的优先级高于"libcrypto.dll"的导入库"libcrypto.lib
        libcrypto${_OPENSSL_STATIC_SUFFIX}${_OPENSSL_MSVC_ARCH_SUFFIX}${_OPENSSL_MSVC_RT_MODE}
        libcrypto${_OPENSSL_STATIC_SUFFIX}${_OPENSSL_MSVC_RT_MODE}
        libcrypto${_OPENSSL_STATIC_SUFFIX}
        libeay32${_OPENSSL_STATIC_SUFFIX}${_OPENSSL_MSVC_RT_MODE}
        libeay32${_OPENSSL_STATIC_SUFFIX}
        crypto${_OPENSSL_STATIC_SUFFIX}
        # 当使用“-static”选项构建OpenSSL时，只生成静态构建，并且不添加“_static”后缀。
        libcrypto${_OPENSSL_MSVC_ARCH_SUFFIX}${_OPENSSL_MSVC_RT_MODE}
        libcrypto${_OPENSSL_MSVC_RT_MODE}
        libcrypto
        libeay32${_OPENSSL_MSVC_RT_MODE}
        libeay32
        crypto
      NAMES_PER_DIR
      ${_OPENSSL_ROOT_HINTS_AND_PATHS}
      PATH_SUFFIXES
        ${_OPENSSL_PATH_SUFFIXES}
    )

    find_library(SSL_EAY_DEBUG
      NAMES
        # 当使用默认选项构建OpenSSL时，静态库名称以“_static”作为后缀。
        # 查看"libssl_static.lib"的优先级高于"libssl.dll"的导入库"libssl.lib
        libssl${_OPENSSL_STATIC_SUFFIX}${_OPENSSL_MSVC_ARCH_SUFFIX}${_OPENSSL_MSVC_RT_MODE}d
        libssl${_OPENSSL_STATIC_SUFFIX}${_OPENSSL_MSVC_RT_MODE}d
        libssl${_OPENSSL_STATIC_SUFFIX}d
        ssleay32${_OPENSSL_STATIC_SUFFIX}${_OPENSSL_MSVC_RT_MODE}d
        ssleay32${_OPENSSL_STATIC_SUFFIX}d
        ssl${_OPENSSL_STATIC_SUFFIX}d
        # 当使用“-static”选项构建OpenSSL时，只生成静态构建，并且不添加“_static”后缀。
        libssl${_OPENSSL_MSVC_ARCH_SUFFIX}${_OPENSSL_MSVC_RT_MODE}d
        libssl${_OPENSSL_MSVC_RT_MODE}d
        libssld
        ssleay32${_OPENSSL_MSVC_RT_MODE}d
        ssleay32d
        ssld
      NAMES_PER_DIR
      ${_OPENSSL_ROOT_HINTS_AND_PATHS}
      PATH_SUFFIXES
        ${_OPENSSL_PATH_SUFFIXES}
    )

    find_library(SSL_EAY_RELEASE
      NAMES
        # 当使用默认选项构建OpenSSL时，静态库名称以“_static”作为后缀。
        # 查看"libssl_static.lib"的优先级高于"libssl.dll"的导入库"libssl.lib
        libssl${_OPENSSL_STATIC_SUFFIX}${_OPENSSL_MSVC_ARCH_SUFFIX}${_OPENSSL_MSVC_RT_MODE}
        libssl${_OPENSSL_STATIC_SUFFIX}${_OPENSSL_MSVC_RT_MODE}
        libssl${_OPENSSL_STATIC_SUFFIX}
        ssleay32${_OPENSSL_STATIC_SUFFIX}${_OPENSSL_MSVC_RT_MODE}
        ssleay32${_OPENSSL_STATIC_SUFFIX}
        ssl${_OPENSSL_STATIC_SUFFIX}
        # 当使用“-static”选项构建OpenSSL时，只生成静态构建，并且不添加“_static”后缀。
        libssl${_OPENSSL_MSVC_ARCH_SUFFIX}${_OPENSSL_MSVC_RT_MODE}
        libssl${_OPENSSL_MSVC_RT_MODE}
        libssl
        ssleay32${_OPENSSL_MSVC_RT_MODE}
        ssleay32
        ssl
      NAMES_PER_DIR
      ${_OPENSSL_ROOT_HINTS_AND_PATHS}
      PATH_SUFFIXES
        ${_OPENSSL_PATH_SUFFIXES}
    )

    set(LIB_EAY_LIBRARY_DEBUG "${LIB_EAY_DEBUG}")
    set(LIB_EAY_LIBRARY_RELEASE "${LIB_EAY_RELEASE}")
    set(SSL_EAY_LIBRARY_DEBUG "${SSL_EAY_DEBUG}")
    set(SSL_EAY_LIBRARY_RELEASE "${SSL_EAY_RELEASE}")

    include(SelectLibraryConfigurations)
    select_library_configurations(LIB_EAY)
    select_library_configurations(SSL_EAY)

    mark_as_advanced(LIB_EAY_LIBRARY_DEBUG LIB_EAY_LIBRARY_RELEASE
                     SSL_EAY_LIBRARY_DEBUG SSL_EAY_LIBRARY_RELEASE)
    set(OPENSSL_SSL_LIBRARY ${SSL_EAY_LIBRARY} )
    set(OPENSSL_CRYPTO_LIBRARY ${LIB_EAY_LIBRARY} )
  elseif(MINGW)
    # same player, for MinGW
    set(LIB_EAY_NAMES crypto libeay32)
    set(SSL_EAY_NAMES ssl ssleay32)
    find_library(LIB_EAY
      NAMES
        ${LIB_EAY_NAMES}
      NAMES_PER_DIR
      ${_OPENSSL_ROOT_HINTS_AND_PATHS}
      PATH_SUFFIXES
        "lib/MinGW"
        "lib"
    )

    find_library(SSL_EAY
      NAMES
        ${SSL_EAY_NAMES}
      NAMES_PER_DIR
      ${_OPENSSL_ROOT_HINTS_AND_PATHS}
      PATH_SUFFIXES
        "lib/MinGW"
        "lib"
    )

    mark_as_advanced(SSL_EAY LIB_EAY)
    set(OPENSSL_SSL_LIBRARY ${SSL_EAY} )
    set(OPENSSL_CRYPTO_LIBRARY ${LIB_EAY} )
    unset(LIB_EAY_NAMES)
    unset(SSL_EAY_NAMES)
  else()
    # 不知道如何选择-比如- intel，让我们使用顶级的，希望有人报告问题:
    find_library(LIB_EAY
      NAMES
        libcrypto
        libeay32
      NAMES_PER_DIR
      ${_OPENSSL_ROOT_HINTS_AND_PATHS}
      HINTS
        ${_OPENSSL_LIBDIR}
      PATH_SUFFIXES
        lib
    )

    find_library(SSL_EAY
      NAMES
        libssl
        ssleay32
      NAMES_PER_DIR
      ${_OPENSSL_ROOT_HINTS_AND_PATHS}
      HINTS
        ${_OPENSSL_LIBDIR}
      PATH_SUFFIXES
        lib
    )

    mark_as_advanced(SSL_EAY LIB_EAY)
    set(OPENSSL_SSL_LIBRARY ${SSL_EAY} )
    set(OPENSSL_CRYPTO_LIBRARY ${LIB_EAY} )
  endif()
else()

  find_library(OPENSSL_SSL_LIBRARY
    NAMES
      ssl
      ssleay32
      ssleay32MD
    NAMES_PER_DIR
    ${_OPENSSL_ROOT_HINTS_AND_PATHS}
    HINTS
      ${_OPENSSL_LIBDIR}
      ${_OPENSSL_LIBRARY_DIRS}
    PATH_SUFFIXES
      lib
  )

  find_library(OPENSSL_CRYPTO_LIBRARY
    NAMES
      crypto
    NAMES_PER_DIR
    ${_OPENSSL_ROOT_HINTS_AND_PATHS}
    HINTS
      ${_OPENSSL_LIBDIR}
      ${_OPENSSL_LIBRARY_DIRS}
    PATH_SUFFIXES
      lib
  )

  mark_as_advanced(OPENSSL_CRYPTO_LIBRARY OPENSSL_SSL_LIBRARY)

endif()
function(from_hex HEX DEC)
  string(TOUPPER "${HEX}" HEX)
  set(_res 0)
  string(LENGTH "${HEX}" _strlen)

  while (_strlen GREATER 0)
    math(EXPR _res "${_res} * 16")
    string(SUBSTRING "${HEX}" 0 1 NIBBLE) 
    string(SUBSTRING "${HEX}" 1 -1 HEX) 
    if (NIBBLE STREQUAL "A")
      math(EXPR _res "${_res} + 10")
    elseif (NIBBLE STREQUAL "B")
      math(EXPR _res "${_res} + 11")
    elseif (NIBBLE STREQUAL "C")
      math(EXPR _res "${_res} + 12")
    elseif (NIBBLE STREQUAL "D")
      math(EXPR _res "${_res} + 13")
    elseif (NIBBLE STREQUAL "E")
      math(EXPR _res "${_res} + 14")
    elseif (NIBBLE STREQUAL "F")
      math(EXPR _res "${_res} + 15")
    else()  
      math(EXPR _res "${_res} + ${NIBBLE}")
    endif() 

    string(LENGTH "${HEX}" _strlen)
  endwhile()

  set(${DEC} ${_res} PARENT_SCOPE)
endfunction(from_hex)

set(OPENSSL_SSL_LIBRARIES ${OPENSSL_SSL_LIBRARY})
set(OPENSSL_CRYPTO_LIBRARIES ${OPENSSL_CRYPTO_LIBRARY})
set(OPENSSL_LIBRARIES ${OPENSSL_SSL_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARIES} )
_OpenSSL_test_and_find_dependencies("${OPENSSL_SSL_LIBRARY}" "${OPENSSL_CRYPTO_LIBRARY}")
if(_OpenSSL_has_dependencies)
  _OpenSSL_add_dependencies( OPENSSL_SSL_LIBRARIES )
  _OpenSSL_add_dependencies( OPENSSL_CRYPTO_LIBRARIES )
  _OpenSSL_add_dependencies( OPENSSL_LIBRARIES )
endif()

function(from_hex HEX DEC)
  string(TOUPPER "${HEX}" HEX)
  set(_res 0)
  string(LENGTH "${HEX}" _strlen)

  while (_strlen GREATER 0)
    math(EXPR _res "${_res} * 16")
    string(SUBSTRING "${HEX}" 0 1 NIBBLE)
    string(SUBSTRING "${HEX}" 1 -1 HEX)
    if (NIBBLE STREQUAL "A")
      math(EXPR _res "${_res} + 10")
    elseif (NIBBLE STREQUAL "B")
      math(EXPR _res "${_res} + 11")
    elseif (NIBBLE STREQUAL "C")
      math(EXPR _res "${_res} + 12")
    elseif (NIBBLE STREQUAL "D")
      math(EXPR _res "${_res} + 13")
    elseif (NIBBLE STREQUAL "E")
      math(EXPR _res "${_res} + 14")
    elseif (NIBBLE STREQUAL "F")
      math(EXPR _res "${_res} + 15")
    else()
      math(EXPR _res "${_res} + ${NIBBLE}")
    endif()

    string(LENGTH "${HEX}" _strlen)
  endwhile()

  set(${DEC} ${_res} PARENT_SCOPE)
endfunction()

if(OPENSSL_INCLUDE_DIR AND EXISTS "${OPENSSL_INCLUDE_DIR}/openssl/opensslv.h")
  file(STRINGS "${OPENSSL_INCLUDE_DIR}/openssl/opensslv.h" openssl_version_str
       REGEX "^#[\t ]*define[\t ]+OPENSSL_VERSION_NUMBER[\t ]+0x([0-9a-fA-F])+.*")

  if(openssl_version_str)
    # 版本号编码为0xMNNFFPPS: 主要次要修复补丁状态
    # 这个状态给出了这是一个开发人员还是一个预发布版本，在这里被忽略。
    # Major, minor和fix直接转换为字符串中显示的版本号。
    # patch字段转换为表示bug修复状态的单个字符后缀，即00 -> nothing、01 -> a、02 -> b等等

    string(REGEX REPLACE "^.*OPENSSL_VERSION_NUMBER[\t ]+0x([0-9a-fA-F])([0-9a-fA-F][0-9a-fA-F])([0-9a-fA-F][0-9a-fA-F])([0-9a-fA-F][0-9a-fA-F])([0-9a-fA-F]).*$"
           "\\1;\\2;\\3;\\4;\\5" OPENSSL_VERSION_LIST "${openssl_version_str}")
    list(GET OPENSSL_VERSION_LIST 0 OPENSSL_VERSION_MAJOR)
    list(GET OPENSSL_VERSION_LIST 1 OPENSSL_VERSION_MINOR)
    from_hex("${OPENSSL_VERSION_MINOR}" OPENSSL_VERSION_MINOR)
    list(GET OPENSSL_VERSION_LIST 2 OPENSSL_VERSION_FIX)
    from_hex("${OPENSSL_VERSION_FIX}" OPENSSL_VERSION_FIX)
    list(GET OPENSSL_VERSION_LIST 3 OPENSSL_VERSION_PATCH)

    if(NOT OPENSSL_VERSION_PATCH STREQUAL "00")
      from_hex("${OPENSSL_VERSION_PATCH}" _tmp)
      #96是‘a’减1的ASCII码
      math(EXPR OPENSSL_VERSION_PATCH_ASCII "${_tmp} + 96")
      unset(_tmp)
      # 一旦任何人知道了OpenSSL如何调用'z'以外的补丁版本，也应该更新来处理。还没有发生，所以它只是暂时被忽略。
      string(ASCII "${OPENSSL_VERSION_PATCH_ASCII}" OPENSSL_VERSION_PATCH_STRING)
    endif ()

    set(OPENSSL_VERSION "${OPENSSL_VERSION_MAJOR}.${OPENSSL_VERSION_MINOR}.${OPENSSL_VERSION_FIX}${OPENSSL_VERSION_PATCH_STRING}")
  else ()
    # 因为OpenSSL 3.0.0，新的版本格式是MAJOR.MINOR.PATCH，新的OPENSSL_VERSION_STR宏就包含了这个
    file(STRINGS "${OPENSSL_INCLUDE_DIR}/openssl/opensslv.h" OPENSSL_VERSION_STR
         REGEX "^#[\t ]*define[\t ]+OPENSSL_VERSION_STR[\t ]+\"([0-9])+\\.([0-9])+\\.([0-9])+\".*")
    string(REGEX REPLACE "^.*OPENSSL_VERSION_STR[\t ]+\"([0-9]+\\.[0-9]+\\.[0-9]+)\".*$"
           "\\1" OPENSSL_VERSION_STR "${OPENSSL_VERSION_STR}")

    set(OPENSSL_VERSION "${OPENSSL_VERSION_STR}")

    unset(OPENSSL_VERSION_STR)
  endif ()
endif ()

foreach(_comp IN LISTS OpenSSL_FIND_COMPONENTS)
  if(_comp STREQUAL "Crypto")
    if(EXISTS "${OPENSSL_INCLUDE_DIR}" AND
        (EXISTS "${OPENSSL_CRYPTO_LIBRARY}" OR
        EXISTS "${LIB_EAY_LIBRARY_DEBUG}" OR
        EXISTS "${LIB_EAY_LIBRARY_RELEASE}")
    )
      set(OpenSSL_${_comp}_FOUND TRUE)
    else()
      set(OpenSSL_${_comp}_FOUND FALSE)
    endif()
  elseif(_comp STREQUAL "SSL")
    if(EXISTS "${OPENSSL_INCLUDE_DIR}" AND
        (EXISTS "${OPENSSL_SSL_LIBRARY}" OR
        EXISTS "${SSL_EAY_LIBRARY_DEBUG}" OR
        EXISTS "${SSL_EAY_LIBRARY_RELEASE}")
    )
      set(OpenSSL_${_comp}_FOUND TRUE)
    else()
      set(OpenSSL_${_comp}_FOUND FALSE)
    endif()
  else()
    message(WARNING "${_comp} is not a valid OpenSSL component")
    set(OpenSSL_${_comp}_FOUND FALSE)
  endif()
endforeach()
unset(_comp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenSSL
  REQUIRED_VARS
    OPENSSL_CRYPTO_LIBRARY
    OPENSSL_INCLUDE_DIR
  VERSION_VAR
    OPENSSL_VERSION
  HANDLE_COMPONENTS
  FAIL_MESSAGE
    "Could NOT find OpenSSL, try to set the path to OpenSSL root folder in the system variable OPENSSL_ROOT_DIR"
)

mark_as_advanced(OPENSSL_INCLUDE_DIR)

if(OPENSSL_FOUND)
  message(STATUS "Found OpenSSL library: ${OPENSSL_LIBRARIES}")
  message(STATUS "Found OpenSSL headers: ${OPENSSL_INCLUDE_DIR}")
  include(EnsureVersion)
  ENSURE_VERSION("${OPENSSL_EXPECTED_VERSION}" "${OPENSSL_VERSION}" OPENSSL_VERSION_OK)
  if(NOT OPENSSL_VERSION_OK)
      message(FATAL_ERROR "RenduCore needs OpenSSL version ${OPENSSL_EXPECTED_VERSION} but found too new version ${OPENSSL_VERSION}. RenduCore needs OpenSSL 1.0.x or 1.1.x to work properly. If you still have problems please install OpenSSL 1.0.x if you still have problems search on forum for TCE00022")
  endif()

  if(NOT TARGET OpenSSL::Crypto AND
      (EXISTS "${OPENSSL_CRYPTO_LIBRARY}" OR
        EXISTS "${LIB_EAY_LIBRARY_DEBUG}" OR
        EXISTS "${LIB_EAY_LIBRARY_RELEASE}")
      )
    add_library(OpenSSL::Crypto UNKNOWN IMPORTED)
    set_target_properties(OpenSSL::Crypto PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}")
    if(EXISTS "${OPENSSL_CRYPTO_LIBRARY}")
      set_target_properties(OpenSSL::Crypto PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${OPENSSL_CRYPTO_LIBRARY}")
    endif()
    if(EXISTS "${LIB_EAY_LIBRARY_RELEASE}")
      set_property(TARGET OpenSSL::Crypto APPEND PROPERTY
        IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(OpenSSL::Crypto PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
        IMPORTED_LOCATION_RELEASE "${LIB_EAY_LIBRARY_RELEASE}")
    endif()
    if(EXISTS "${LIB_EAY_LIBRARY_DEBUG}")
      set_property(TARGET OpenSSL::Crypto APPEND PROPERTY
        IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(OpenSSL::Crypto PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
        IMPORTED_LOCATION_DEBUG "${LIB_EAY_LIBRARY_DEBUG}")
    endif()
    _OpenSSL_target_add_dependencies(OpenSSL::Crypto)
  endif()

  if(NOT TARGET OpenSSL::SSL AND
      (EXISTS "${OPENSSL_SSL_LIBRARY}" OR
        EXISTS "${SSL_EAY_LIBRARY_DEBUG}" OR
        EXISTS "${SSL_EAY_LIBRARY_RELEASE}")
      )
    add_library(OpenSSL::SSL UNKNOWN IMPORTED)
    set_target_properties(OpenSSL::SSL PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}")
    if(EXISTS "${OPENSSL_SSL_LIBRARY}")
      set_target_properties(OpenSSL::SSL PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${OPENSSL_SSL_LIBRARY}")
    endif()
    if(EXISTS "${SSL_EAY_LIBRARY_RELEASE}")
      set_property(TARGET OpenSSL::SSL APPEND PROPERTY
        IMPORTED_CONFIGURATIONS RELEASE)
      set_target_properties(OpenSSL::SSL PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
        IMPORTED_LOCATION_RELEASE "${SSL_EAY_LIBRARY_RELEASE}")
    endif()
    if(EXISTS "${SSL_EAY_LIBRARY_DEBUG}")
      set_property(TARGET OpenSSL::SSL APPEND PROPERTY
        IMPORTED_CONFIGURATIONS DEBUG)
      set_target_properties(OpenSSL::SSL PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
        IMPORTED_LOCATION_DEBUG "${SSL_EAY_LIBRARY_DEBUG}")
    endif()
    if(TARGET OpenSSL::Crypto)
      set_target_properties(OpenSSL::SSL PROPERTIES
        INTERFACE_LINK_LIBRARIES OpenSSL::Crypto)
    endif()
    _OpenSSL_target_add_dependencies(OpenSSL::SSL)
  endif()

  if("${OPENSSL_VERSION_MAJOR}.${OPENSSL_VERSION_MAJOR}.${OPENSSL_VERSION_FIX}" VERSION_GREATER_EQUAL "0.9.8")
    if(MSVC)
      if(EXISTS "${OPENSSL_INCLUDE_DIR}")
        set(_OPENSSL_applink_paths PATHS ${OPENSSL_INCLUDE_DIR})
      endif()
      find_file(OPENSSL_APPLINK_SOURCE
        NAMES
          openssl/applink.c
        ${_OPENSSL_applink_paths}
        NO_DEFAULT_PATH)
      if(OPENSSL_APPLINK_SOURCE)
        set(_OPENSSL_applink_interface_srcs ${OPENSSL_APPLINK_SOURCE})
      endif()
    endif()
    if(NOT TARGET OpenSSL::applink)
      add_library(OpenSSL::applink INTERFACE IMPORTED)
      set_property(TARGET OpenSSL::applink APPEND
        PROPERTY INTERFACE_SOURCES
          ${_OPENSSL_applink_interface_srcs})
    endif()
  endif()
endif()

# 恢复原来的查找库顺序
if(OPENSSL_USE_STATIC_LIBS)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ${_openssl_ORIG_CMAKE_FIND_LIBRARY_SUFFIXES})
endif()
