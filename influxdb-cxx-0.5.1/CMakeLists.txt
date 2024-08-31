####################################
# General project definition
####################################

CMAKE_MINIMUM_REQUIRED(VERSION 3.5.2 FATAL_ERROR)

option(BUILD_SHARED_LIBS "Build shared versions of libraries" OFF)
option(INFLUXCXX_TESTING "Enable testing for this component" OFF)

# Set cmake policy by version: https://cmake.org/cmake/help/latest/manual/cmake-policies.7.html
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.12)
endif()

# Define project
project(influxdb-cxx
  VERSION 0.0.1
  DESCRIPTION "InfluxDB C++ client library"
  LANGUAGES CXX
)

# Documentation dir
#add_subdirectory(doc)

# Add compiler flags for warnings and debug symbols
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

# Set fPIC for all targets
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set CMAKE_INSTALL_LIBDIR explicitly to lib (to avoid lib64 on CC7)
set(CMAKE_INSTALL_LIBDIR lib)

# Set the default build type to "RelWithDebInfo"
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "RelWithDebInfo"
    CACHE
    STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel Coverage."
    FORCE
  )
endif()

# Add coverage flags to Debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage")
endif()

# Fixes warning when using boost from brew
set(Boost_USE_MULTITHREADED TRUE)

####################################
# Dependencies
####################################

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(Boost COMPONENTS unit_test_framework system program_options)
find_package(CURL REQUIRED MODULE)


####################################
# Library
####################################

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(INCLUDE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/include")

# Create library
# note: BUILD_SHARED_LIBS specifies if static or shared
#       as boost is build without -fPIC, we cannot
#       statically link against it when building
#       influxdb as shared object
add_library(InfluxDB
  src/InfluxDB.cxx
  src/Point.cxx
  src/InfluxDBFactory.cxx
  $<$<BOOL:${Boost_FOUND}>:src/UDP.cxx>
  $<$<BOOL:${Boost_FOUND}>:src/UnixSocket.cxx>
  src/HTTP.cxx
)
target_include_directories(InfluxDB
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link targets
target_link_libraries(InfluxDB
  PRIVATE
    $<$<BOOL:${Boost_FOUND}>:Boost::system>
    CURL::libcurl
)

# Use C++17
target_compile_features(InfluxDB PUBLIC cxx_std_17)

# Set compile definition if Boost found
target_compile_definitions(InfluxDB
  PRIVATE
    $<$<BOOL:${Boost_FOUND}>:INFLUXDB_WITH_BOOST>
)

####################################
# Tests
####################################

# Tests require Boost::unit_test_framework
if (Boost_FOUND AND INFLUXCXX_TESTING)

  enable_testing()

  set(TEST_SRCS
    test/testUdp.cxx
    test/testPoint.cxx
    test/testHttp.cxx
    test/testQuery.cxx
    test/testFactory.cxx
  )

  foreach (test ${TEST_SRCS})
    get_filename_component(test_name ${test} NAME)
    string(REGEX REPLACE ".cxx" "" test_name ${test_name})

    add_executable(${test_name} ${test})
    target_link_libraries(${test_name}
      PRIVATE
        InfluxDB Boost::unit_test_framework
    )
    add_test(NAME ${test_name} COMMAND ${test_name})
    set_tests_properties(${test_name} PROPERTIES TIMEOUT 60)
  endforeach()

  add_executable(benchmark test/benchmark.cxx)
  target_link_libraries(benchmark PRIVATE InfluxDB Boost::program_options)
endif()


####################################
# Install
####################################

include(GNUInstallDirs)

# Build targets with install rpath on Mac to dramatically speed up installation
# https://gitlab.kitware.com/cmake/community/wikis/doc/cmake/RPATH-handling
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}" isSystemDir)
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  if("${isSystemDir}" STREQUAL "-1")
    set(CMAKE_INSTALL_RPATH "@loader_path/../${CMAKE_INSTALL_LIBDIR}")
  endif()
  set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
endif()
unset(isSystemDir)

# Install library
install(TARGETS InfluxDB
  EXPORT InfluxDBTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Create version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/cmake/InfluxDBConfigVersion.cmake"
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY AnyNewerVersion
)

# Install headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

# Export targets
install(EXPORT InfluxDBTargets
  FILE
    InfluxDBTargets.cmake
  NAMESPACE
    InfluxData::
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/InfluxDB
)

# Configure and install Config files
configure_package_config_file(
  cmake/InfluxDBConfig.cmake.in cmake/InfluxDBConfig.cmake
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/InfluxDB"
  PATH_VARS CMAKE_INSTALL_PREFIX
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/InfluxDBConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/InfluxDBConfigVersion.cmake"
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/InfluxDB
)
