cmake_minimum_required(VERSION 3.21)

project(saneqc LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/fast_float)

option(SANEQC_USE_SYSTEM_PEGTL "Use system PEGTL" OFF)
if(SANEQC_USE_SYSTEM_PEGTL)
  find_package(pegtl REQUIRED)
else()
  add_subdirectory(external/PEGTL)
endif()

add_subdirectory(src)

option(SANEQC_BUILD_TESTS "Build tests" ON)
if(SANEQC_BUILD_TESTS)
  enable_testing()
  add_subdirectory(src/test)
endif()

option(SANEQC_USE_QCINT "Use qcint rather than libcint" ON)

if(SANEQC_USE_QCINT)
  add_subdirectory(external/qcint)
else()
  add_subdirectory(external/libcint)
endif()
