# 增强路径空格检查功能
function(WarnAboutSpacesInBuildPath)
  if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    if("${CMAKE_BINARY_DIR}" MATCHES "[ ]+")
      message(WARNING [[
        ███████╗██████╗  █████╗  ██████╗███████╗
        ██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝
        ███████╗██████╔╝███████║██║     █████╗
        ╚════██║██╔═══╝ ██╔══██║██║     ██╔══╝
        ███████║██║     ██║  ██║╚██████╗███████╗
        ╚══════╝╚═╝     ╚═╝  ╚═╝ ╚═════╝╚══════╝

        构建路径包含空格：${CMAKE_BINARY_DIR}
        可能引发问题：
        1. 编译工具链兼容性问题
        2. 安装路径解析异常
        3. 脚本加载失败]])
    endif()
  endif()
endfunction()