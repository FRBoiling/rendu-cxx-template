#**********************************
#  Created by boil on 2022/8/14.
#**********************************

# 这个文件定义了以下宏，供开发人员使用，以确保安装的软件版本正确:
#
# NORMALIZE_VERSION
#   帮助宏，将形式"x.y.z"的版本号转换为等于10^4 * x + 10^2 * y + z的整数
#   如果x,y,z中的任何一个大于100，这个宏将会静默中断。
MACRO(NORMALIZE_VERSION _requested_version _normalized_version)
    STRING(REGEX MATCH "[^0-9]*[0-9]+\\.[0-9]+\\.[0-9]+.*" _threePartMatch "${_requested_version}")
    if(_threePartMatch)
    # 解析版本字符串的各个部分
        STRING(REGEX REPLACE "[^0-9]*([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" _major_vers "${_requested_version}")
        STRING(REGEX REPLACE "[^0-9]*[0-9]+\\.([0-9]+)\\.[0-9]+.*" "\\1" _minor_vers "${_requested_version}")
        STRING(REGEX REPLACE "[^0-9]*[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" _patch_vers "${_requested_version}")
    else(_threePartMatch)
        STRING(REGEX REPLACE "([0-9]+)\\.[0-9]+" "\\1" _major_vers "${_requested_version}")
        STRING(REGEX REPLACE "[0-9]+\\.([0-9]+)" "\\1" _minor_vers "${_requested_version}")
        set(_patch_vers "0")
    endif(_threePartMatch)
    # 计算可以一次比较的整体版本号
    MATH(EXPR ${_normalized_version} "${_major_vers}*10000 + ${_minor_vers}*100 + ${_patch_vers}")
ENDMACRO(NORMALIZE_VERSION)

# CHECK_RANGE_INCLUSIVE_LOWER
#   帮助宏检查 x <= y < z
MACRO(CHECK_RANGE_INCLUSIVE_LOWER _lower_limit _value _upper_limit _ok)
   if(${_value} LESS ${_lower_limit})
      set(${_ok} FALSE)
  elseif(${_value} EQUAL ${_lower_limit})
      set(${_ok} TRUE)
  elseif(${_value} EQUAL ${_upper_limit})
      set(${_ok} FALSE)
  elseif(${_value} GREATER ${_upper_limit})
      set(${_ok} FALSE)
  else(${_value} LESS ${_lower_limit})
      set(${_ok} TRUE)
  endif(${_value} LESS ${_lower_limit})
ENDMACRO(CHECK_RANGE_INCLUSIVE_LOWER)

# ENSURE_VERSION        - 测试版本号是否大于或等于某个最小值
# ENSURE_VERSION(FOO_MIN_VERSION FOO_VERSION_FOUND FOO_VERSION_OK)
#   将设置FOO_VERSION_OK为true如果FOO_VERSION_FOUND >= FOO_MIN_VERSION开头和结尾的文本是ok的，
# 例如，
#   ENSURE_VERSION("2.5.31" "flex 2.5.4a" VERSION_OK)
#   意味着2.5.31是必需的，“flex 2.5.4a”是在系统上发现的
#   这个宏比较表单的版本号 "x.y.z" or "x.y"
MACRO(ENSURE_VERSION requested_version found_version var_too_old)
    NORMALIZE_VERSION(${requested_version} req_vers_num)
    NORMALIZE_VERSION(${found_version} found_vers_num)

    if(found_vers_num LESS req_vers_num)
        set(${var_too_old} FALSE)
    else(found_vers_num LESS req_vers_num)
        set(${var_too_old} TRUE)
    endif(found_vers_num LESS req_vers_num)

ENDMACRO(ENSURE_VERSION)

# ENSURE_VERSION_RANGE  - 测试版本号是否大于或等于某个最小值且小于某个最大
#   这个宏确保表单“x.y.z”或“x.y”的版本号在定义的范围内 min_version <= found_version < max_version.
#   如果这个表达式成立，FOO_VERSION_OK 会被设置成 TRUE
# 例如，
#   ENSURE_VERSION_RANGE3("0.1.0" ${FOOCODE_VERSION} "0.7.0" FOO_VERSION_OK)
#   如果x,y,z中的任何一个大于100，这个宏将会静默中断。
MACRO(ENSURE_VERSION_RANGE min_version found_version max_version var_ok)
    NORMALIZE_VERSION(${min_version} req_vers_num)
    NORMALIZE_VERSION(${found_version} found_vers_num)
    NORMALIZE_VERSION(${max_version} max_vers_num)

    CHECK_RANGE_INCLUSIVE_LOWER(${req_vers_num} ${found_vers_num} ${max_vers_num} ${var_ok})
ENDMACRO(ENSURE_VERSION_RANGE)
