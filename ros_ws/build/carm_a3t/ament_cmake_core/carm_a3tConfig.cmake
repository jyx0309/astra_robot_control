# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_carm_a3t_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED carm_a3t_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(carm_a3t_FOUND FALSE)
  elseif(NOT carm_a3t_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(carm_a3t_FOUND FALSE)
  endif()
  return()
endif()
set(_carm_a3t_CONFIG_INCLUDED TRUE)

# output package information
if(NOT carm_a3t_FIND_QUIETLY)
  message(STATUS "Found carm_a3t: 0.0.0 (${carm_a3t_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'carm_a3t' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT carm_a3t_DEPRECATED_QUIET)
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(carm_a3t_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${carm_a3t_DIR}/${_extra}")
endforeach()
