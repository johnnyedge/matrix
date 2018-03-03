cmake_minimum_required(VERSION 3.10)

include(ExternalProject)
set(GTEST_PREFIX ${CMAKE_BINARY_DIR}/gtest)
set(GTEST_INSTALL_DIR ${GTEST_PREFIX}/install)
ExternalProject_Add(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG master
  PREFIX ${GTEST_PREFIX}
  CMAKE_CACHE_DEFAULT_ARGS -DCMAKE_INSTALL_PREFIX:STRING=${GTEST_INSTALL_DIR}
  LOG_INSTALL 1)

add_library(gtest STATIC IMPORTED)
set_target_properties(
  gtest
  PROPERTIES
  IMPORTED_LOCATION ${GTEST_INSTALL_DIR}/lib/libgtest.a
  INTERFACE_INCLUDE_DIRECTORIES ${GTEST_INSTALL_DIR}/include)
add_library(gtest_main STATIC IMPORTED)
set_target_properties(
  gtest_main
  PROPERTIES
  IMPORTED_LOCATION ${GTEST_INSTALL_DIR}/lib/libgtest_main.a)

add_executable(
  unittest
  EXCLUDE_FROM_ALL
  unittest.cpp)

target_link_libraries(
  unittest
  gtest_main gtest pthread)

add_custom_target(
  run_tests
  unittest)
