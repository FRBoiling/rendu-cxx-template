# cmake/Utils/ShowMessage.cmake
include(Colors)

# ======================= 输出宏定义 ======================= #
function(printf_header message color)
  message("${color}${message}${ColorReset}")
endfunction()

function(print_section title)
  message("\n${Bold}${Cyan}◼ ${title}${ColorReset}")
endfunction()

function(print_option msg value color)
  message("  ${Bold}├─ ${msg} : ${color}${value}${ColorReset}")
endfunction()

function(print_final_option msg value color)
  message("  ${Bold}└─ ${msg} : ${color}${value}${ColorReset}")
endfunction()

function(print_switch msg var enabled_color enabled_symbol disabled_color disabled_symbol)
  if (${var})
    print_option("${msg}" "${enabled_symbol}" ${enabled_color})
  else ()
    print_option("${msg}" "${disabled_symbol}" ${disabled_color})
  endif ()
endfunction()

function(print_final_switch msg var enabled_color enabled_symbol disabled_color disabled_symbol)
  if (${var})
    print_final_option("${msg}" "${enabled_symbol}" ${enabled_color})
  else ()
    print_final_option("${msg}" "${disabled_symbol}" ${disabled_color})
  endif ()
endfunction()

