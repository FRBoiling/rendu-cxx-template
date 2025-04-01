#**********************************
#  Created by boil on 2025/02/21.
#**********************************

include(ShowMessage)

# 用户手动禁用git检测时显示警告
# 每次编译都会显示以提醒可能产生的影响
if (NOT BUILDDIR)
  # 针对CMake脚本模式的临时解决方案
  set(BUILDDIR ${CMAKE_BINARY_DIR})
endif ()

print_section("版本配置")

if (NOT RENDU_DISABLE_GIT_HOOKS)
  set(rev_date "1970-01-01 00:00:00 +0000")
  set(rev_hash "unknown")
  set(rev_branch "Archived")
  # 没有有效的git提交日期，使用当前时间
  string(TIMESTAMP rev_date_fallback "%Y-%m-%d %H:%M:%S" UTC)
else ()
  if (GIT_EXECUTABLE)
    # 生成版本哈希字符串
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" rev-parse --short=12 HEAD
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE rev_hash
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if (rev_hash)
      # 检测仓库未提交更改状态
      execute_process(
          COMMAND "${GIT_EXECUTABLE}" diff-index --quiet HEAD --
          WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
          RESULT_VARIABLE is_dirty
          ERROR_QUIET
      )

      # 添加未提交标记到哈希值
      if (is_dirty)
        set(rev_hash "${rev_hash}+")
      endif ()

      # 获取提交时间戳
      execute_process(
          COMMAND "${GIT_EXECUTABLE}" show -s --format=%ci
          WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
          OUTPUT_VARIABLE rev_date
          OUTPUT_STRIP_TRAILING_WHITESPACE
          ERROR_QUIET
      )

      # 获取分支名称
      execute_process(
          COMMAND "${GIT_EXECUTABLE}" symbolic-ref -q --short HEAD
          WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
          OUTPUT_VARIABLE rev_branch
          OUTPUT_STRIP_TRAILING_WHITESPACE
          ERROR_QUIET
      )

      # 在CI环境下处理分离HEAD状态，尝试扫描本地分支
      if (NOT rev_branch)
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" for-each-ref --points-at=HEAD refs/heads "--format=%(refname:short)"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            OUTPUT_VARIABLE rev_branch
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
      endif ()

      # 如果本地分支扫描无结果，尝试远程分支
      if (NOT rev_branch)
        execute_process(
            COMMAND "${GIT_EXECUTABLE}" for-each-ref --points-at=HEAD refs/remotes "--format=%(refname:short)"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            OUTPUT_VARIABLE rev_branch
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
        )
      endif ()

      # 最终回退方案：使用提交哈希作为分支名
      if (NOT rev_branch)
        set(rev_branch ${rev_hash})
      endif ()

      # 规范化分支名为单行格式（当多个分支指向相同提交时）
      string(REGEX MATCH "^[^ \t\r\n]+" rev_branch ${rev_branch})
    endif ()
  endif ()

  # 最终检查 - 确保有效的版本信息
  # 如果上述步骤都失败（意味着用户删除了git仓库或origin/HEAD标签）
  if (NOT rev_hash)
    # 强制使用默认值
    print_option("Git" "无法获取有效的仓库签名（哈希值) 可能需要执行 git fetch -t 拉取标签" ${Green})
    print_option("继续处理" "版本字符串将被设置为 \"unknown 1970-01-01 00:00:00 (Archived)\"" ${Green})
    set(rev_date "1970-01-01 00:00:00 +0000")
    set(rev_hash "unknown")
    set(rev_branch "Archived")
    string(TIMESTAMP rev_date_fallback "%Y-%m-%d %H:%M:%S" UTC)
  else ()
    # 使用有效的git提交日期
    set(rev_date_fallback ${rev_date})
  endif ()
endif ()

# 为版权信息处理日期格式（保持"Archived/1970"显示但需要有效年份）
string(REGEX MATCH "([0-9]+)-([0-9]+)-([0-9]+)" rev_date_fallback_match ${rev_date_fallback})
set(rev_year ${CMAKE_MATCH_1})
set(rev_month ${CMAKE_MATCH_2})
set(rev_day ${CMAKE_MATCH_3})

# 生成版本信息头文件
cmake_host_system_information(RESULT RENDU_BUILD_HOST_SYSTEM QUERY OS_NAME)
cmake_host_system_information(RESULT RENDU_BUILD_HOST_DISTRO QUERY DISTRIB_INFO)
cmake_host_system_information(RESULT RENDU_BUILD_HOST_SYSTEM_RELEASE QUERY OS_RELEASE)

# Windows系统特殊处理：OS_RELEASE包含专业版等描述信息而非版本号
if (WIN32)
  execute_process(
      COMMAND powershell -NoProfile -Command "$v=(Get-CimInstance -ClassName Win32_OperatingSystem); '{0} ({1})' -f $v.Caption, $v.Version"
      OUTPUT_VARIABLE RENDU_BUILD_HOST_SYSTEM_RELEASE
      OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  # 移除冗余的"Microsoft Windows"前缀
  string(REGEX REPLACE "^.* Windows " "" RENDU_BUILD_HOST_SYSTEM_RELEASE ${RENDU_BUILD_HOST_SYSTEM_RELEASE})
endif ()

if (CMAKE_SCRIPT_MODE_FILE)
  # 临时解决方案：脚本模式下缺少CMAKE_SYSTEM_PROCESSOR变量
  set(CMAKE_PLATFORM_INFO_DIR ${BUILDDIR}${CMAKE_FILES_DIRECTORY})
  include(${CMAKE_ROOT}/Modules/CMakeDetermineSystem.cmake)
endif ()

# 配置生成版本头文件
configure_file(
    "${CMAKE_SOURCE_DIR}/revision_data.h.in.cmake"
    "${BUILDDIR}/revision_data.h"
    @ONLY
)

print_final_option("版本配置" "✔ 已加载" ${Cyan})
