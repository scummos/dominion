set(QWT_DIR "C:\\Qwt-6.1.3" CACHE FILEPATH "Path to Qwt installation directory")

find_path(QWT_INCLUDE_DIRS
  NAMES qwt_global.h
  HINTS ${QWT_DIR}/include
  PATHS /usr/local/include/qwt /usr/include/qwt
)

if(WIN32)
    set(QWT_LIBRARY_RELEASE ${QWT_DIR}/lib/qwt.dll)
    set(QWT_LIBRARY_DEBUG ${QWT_DIR}/lib/qwtd.dll)
    set(QWT_LIBRARY_IMP_RELEASE ${QWT_DIR}/lib/qwt.lib)
    set(QWT_LIBRARY_IMP_DEBUG ${QWT_DIR}/lib/qwtd.lib)
else()
    find_library(QWT_LIBRARY NAMES qwt PATHS /usr/lib /usr/local/lib)
    set(QWT_LIBRARY_RELEASE ${QWT_LIBRARY})
    set(QWT_LIBRARY_DEBUG ${QWT_LIBRARY})
    # Linux does not need imp libs
    set(QWT_LIBRARY_IMP_DEBUG "")
    set(QWT_LIBRARY_IMP_RELEASE "")
endif()

if(QWT_INCLUDE_DIRS)
  file(STRINGS "${QWT_INCLUDE_DIRS}/qwt_global.h"
    QWT_STRING_VERSION REGEX "QWT_VERSION_STR")
  set(QWT_WRONG_VERSION True)
  set(QWT_VERSION "No Version")
  string(REGEX MATCH "[0-9]+.[0-9]+.[0-9]+" QWT_VERSION ${QWT_STRING_VERSION})
  string(COMPARE LESS ${QWT_VERSION} "5.2.0" QWT_WRONG_VERSION)
  string(COMPARE GREATER ${QWT_VERSION} "6.2.0" QWT_WRONG_VERSION)

  message(STATUS "QWT Version: ${QWT_VERSION}")
  if(QWT_WRONG_VERSION)
    message(STATUS "QWT Version must be >= 5.2 and <= 6.2.0, Found ${QWT_VERSION}")
  endif()
endif(QWT_INCLUDE_DIRS)

if(NOT QWT_WRONG_VERSION)
  # handle the QUIETLY and REQUIRED arguments and set Qwt_FOUND to TRUE if
  # all listed variables are TRUE
  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(Qwt
      FOUND_VAR Qwt_FOUND
      REQUIRED_VARS QWT_LIBRARY_DEBUG QWT_LIBRARY_RELEASE QWT_INCLUDE_DIRS
  )
  mark_as_advanced(QWT_LIBRARY_DEBUG QWT_LIBRARY_RELEASE QWT_INCLUDE_DIRS)

  add_library(QwtLib SHARED IMPORTED GLOBAL)
  set_target_properties(QwtLib PROPERTIES IMPORTED_LOCATION_DEBUG "${QWT_LIBRARY_DEBUG}")
  set_target_properties(QwtLib PROPERTIES IMPORTED_LOCATION "${QWT_LIBRARY_RELEASE}")
  set_target_properties(QwtLib PROPERTIES IMPORTED_IMPLIB_DEBUG "${QWT_LIBRARY_IMP_DEBUG}")
  set_target_properties(QwtLib PROPERTIES IMPORTED_IMPLIB "${QWT_LIBRARY_IMP_RELEASE}")
  set_target_properties(QwtLib PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${QWT_INCLUDE_DIRS})
  target_compile_definitions(QwtLib INTERFACE -DQWT_DLL)
endif()
