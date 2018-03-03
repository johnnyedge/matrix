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
file(MAKE_DIRECTORY ${GTEST_INSTALL_DIR}/include)

function(import_googletest BASE_NAME INSTALL_DIR)
add_library(${BASE_NAME} STATIC IMPORTED)
set_target_properties(
  ${BASE_NAME}
  PROPERTIES
  IMPORTED_LOCATION ${INSTALL_DIR}/lib/lib${BASE_NAME}.a
  INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)
add_dependencies(${BASE_NAME} googletest)
endfunction(import_googletest)

import_googletest(gtest ${GTEST_INSTALL_DIR})
import_googletest(gtest_main ${GTEST_INSTALL_DIR})

project(matrix LANGUAGES CXX)

add_executable(unittest EXCLUDE_FROM_ALL unittest.cpp)
set_property(TARGET unittest PROPERTY CXX_STANDARD 11)
target_link_libraries(unittest gtest_main gtest pthread)
add_custom_target(run_tests ALL unittest)

install(
  FILES matrix.h matrix.tpp
  DESTINATION include)
