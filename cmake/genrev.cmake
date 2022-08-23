#**********************************
#  Created by boil on 2022/8/14.
#**********************************

# 用户已经手动选择忽略git测试，所以给他们抛出一个警告。
# 在每次编译时都要这样做，这样他们就可以收到关于后果的警告。
if(NOT BUILDDIR)
  # 有趣的MSVC行为的解决方案-这个段只在使用cmake gui时使用
  set(BUILDDIR ${CMAKE_BINARY_DIR})
endif()

if(WITHOUT_GIT)
  set(rev_date "1970-01-01 00:00:00 +0000")
  set(rev_hash "unknown")
  set(rev_branch "Archived")
  # 没有有效的git提交日期，使用今天
  string(TIMESTAMP rev_date_fallback "%Y-%m-%d %H:%M:%S" UTC)
else()
  if(GIT_EXECUTABLE)
    # 检索存储库脏状态
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" diff-index --quiet HEAD --
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      RESULT_VARIABLE is_dirty
    )

    # 创建一个我们可以使用的revision-string
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" rev-parse --short=12 HEAD
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      OUTPUT_VARIABLE rev_hash
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET
    )

    # 追加dirty标记以提交hash
    if(is_dirty)
      set(rev_hash "${rev_hash}+")
    endif()

    # 并获取提交时间戳
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" show -s --format=%ci
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      OUTPUT_VARIABLE rev_date
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET
    )

    # 还要检索分支名称
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" symbolic-ref -q --short HEAD
      WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
      OUTPUT_VARIABLE rev_branch
      OUTPUT_STRIP_TRAILING_WHITESPACE
      ERROR_QUIET
    )

    # 当在CI上运行时，存储库将处于分离的HEAD状态，尝试扫描已知的本地分支
    if(NOT rev_branch)
      execute_process(
        COMMAND "${GIT_EXECUTABLE}" for-each-ref --points-at=HEAD refs/heads "--format=%(refname:short)"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE rev_branch
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
      )
    endif()

    # 如果本地分支扫描没有找到任何东西，请尝试远程分支
    if(NOT rev_branch)
      execute_process(
        COMMAND "${GIT_EXECUTABLE}" for-each-ref --points-at=HEAD refs/remotes "--format=%(refname:short)"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE rev_branch
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
      )
    endif()

    # 放弃为分支查找名称，使用提交hash
    if(NOT rev_branch)
      set(rev_branch ${rev_hash})
    endif()
    # 将分支规范化为单行(如果同一个提交上有多个分支，for-each-ref可以输出多行)
    string(REGEX MATCH "^[^ \t\r\n]+" rev_branch ${rev_branch})
  endif()

  # 最后一分钟检查-确保我们有一个适当的修改
  # 如果以上都失败了(意味着用户删除了git版本控制目录或删除了origin/HEAD标签)
  if(NOT rev_hash)
    # 没有有效的方法可以 find/set revision/hash ，所以让我们强制一些默认值
    message(STATUS "找不到正确的库签名(散列)-你可能需要用git fetch -t来拉标签，继续-注意versionstring将被设置为\"unknown 1970-01-01 00:00:00 (Archived)\"")
    set(rev_date "1970-01-01 00:00:00 +0000")
    set(rev_hash "unknown")
    set(rev_branch "Archived")
    # 没有有效的git提交日期，使用今天
    string(TIMESTAMP rev_date_fallback "%Y-%m-%d %H:%M:%S" UTC)
  else()
    # 我们有有效日期，从git提交，使用那个
    set(rev_date_fallback ${rev_date})
  endif()
endif()

# 对于包/版权信息，我们总是需要一个适当的日期-保留“Archived/1970”来显示git信息，但在其他地方是有效的年份
string(REGEX MATCH "([0-9]+)-([0-9]+)-([0-9]+)" rev_date_fallback_match ${rev_date_fallback})
set(rev_year ${CMAKE_MATCH_1})
set(rev_month ${CMAKE_MATCH_2})
set(rev_day ${CMAKE_MATCH_3})

# 根据上面的参数创建实际的revision_data.h文件
if(NOT "${rev_hash_cached}" STREQUAL "${rev_hash}" OR NOT "${rev_branch_cached}" STREQUAL "${rev_branch}" OR NOT EXISTS "${BUILDDIR}/revision_data.h")
  configure_file(
    "${CMAKE_SOURCE_DIR}/revision_data.h.in.cmake"
    "${BUILDDIR}/revision_data.h"
    @ONLY
  )
  set(rev_hash_cached "${rev_hash}" CACHE INTERNAL "Cached commit-hash")
  set(rev_branch_cached "${rev_branch}" CACHE INTERNAL "Cached branch name")
endif()
