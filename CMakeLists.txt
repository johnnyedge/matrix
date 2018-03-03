cmake_minimum_required(VERSION 3.10)

include(ExternalProject)

#
# locations into which to download and install our
# local version of googletest
#
set(GTEST_PREFIX ${CMAKE_BINARY_DIR}/gtest)
set(GTEST_INSTALL_DIR ${GTEST_PREFIX}/install)

#
# pull googletest directly out github and build it
#
ExternalProject_Add(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG master
  PREFIX ${GTEST_PREFIX}
  CMAKE_CACHE_DEFAULT_ARGS -DCMAKE_INSTALL_PREFIX:STRING=${GTEST_INSTALL_DIR}
  LOG_INSTALL 1)
file(MAKE_DIRECTORY ${GTEST_INSTALL_DIR}/include)

#
# we have to manually import the files generated
# by the googletest build. googletest builds into
# a couple of libraries. this function allows us
# to easily import them without having to repeat
# a bunch of the same commands
#
function(import_googletest BASE_NAME INSTALL_DIR)
add_library(${BASE_NAME} STATIC IMPORTED)
set_target_properties(
  ${BASE_NAME}
  PROPERTIES
  IMPORTED_LOCATION ${INSTALL_DIR}/lib/lib${BASE_NAME}.a
  INTERFACE_INCLUDE_DIRECTORIES ${INSTALL_DIR}/include)
add_dependencies(${BASE_NAME} googletest)
endfunction(import_googletest)

#
# import the required googletest libraries
#
import_googletest(gtest ${GTEST_INSTALL_DIR})
import_googletest(gtest_main ${GTEST_INSTALL_DIR})

#
# now, we can finally define our actual
# project (which is written in c++)
#

project(matrix LANGUAGES CXX)

#
# add the unittest executable which requires c++11 and
# builds against the gtest, gtest_main, and pthread libraries
#
add_executable(unittest EXCLUDE_FROM_ALL unittest.cpp)
set_property(TARGET unittest PROPERTY CXX_STANDARD 11)
target_link_libraries(unittest gtest_main gtest pthread)

#
# always run the unit tests as part of the build
#
add_custom_target(run_tests ALL unittest)

#
# generate html documentation using doxygen
# try to open the user's web browser using xdg-open
# to display the documentation
#
add_custom_target(
  doxygen
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/doxygen
  COMMAND doxygen Doxyfile
  COMMAND xdg-open ${CMAKE_BINARY_DIR}/doxygen/html/classmatrix.html
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

#
# add a target to install the library. due to the templated
# nature of the matrix object, all functionality is inline
# and no library is generated, i.e. only headers are required.
#
install(
  FILES matrix.h matrix.tpp
  DESTINATION include)
