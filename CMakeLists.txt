cmake_minimum_required(VERSION 3.21)

project(saneqc LANGUAGES C CXX)

add_subdirectory(external/fast_float)
add_subdirectory(external/PEGTL)

add_subdirectory(src)

option(SANEQC_BUILD_TESTS "Build tests" ON)
if(SANEQC_BUILD_TESTS)
  enable_testing()
  add_subdirectory(src/test)
endif()