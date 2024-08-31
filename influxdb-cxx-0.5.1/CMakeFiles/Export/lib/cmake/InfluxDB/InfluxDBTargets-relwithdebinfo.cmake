#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "InfluxData::InfluxDB" for configuration "RelWithDebInfo"
set_property(TARGET InfluxData::InfluxDB APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(InfluxData::InfluxDB PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/libInfluxDB.so"
  IMPORTED_SONAME_RELWITHDEBINFO "libInfluxDB.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS InfluxData::InfluxDB )
list(APPEND _IMPORT_CHECK_FILES_FOR_InfluxData::InfluxDB "${_IMPORT_PREFIX}/lib/libInfluxDB.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
