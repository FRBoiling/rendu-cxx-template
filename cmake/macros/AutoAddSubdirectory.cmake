#**********************************
#  Created by boil on 2022/8/14.
#**********************************

function(AddSubdirectory current_dir)
  list(FIND ARGN "${current_dir}" IS_EXCLUDED)
  if(IS_EXCLUDED EQUAL -1)
    list(APPEND DIRS ${current_dir})
    file(GLOB SUB_DIRECTORIES ${current_dir}/*)
    foreach(SUB_DIRECTORY ${SUB_DIRECTORIES})
      if(IS_DIRECTORY ${SUB_DIRECTORY})
        list(FIND ARGN "${SUB_DIRECTORY}" IS_EXCLUDED)
        if(IS_EXCLUDED EQUAL -1)
          get_filename_component(element_name ${SUB_DIRECTORY} NAME)
#          message(${element_name})
          add_subdirectory(${element_name})
        endif()
      endif()
    endforeach()
  endif()
endfunction()

