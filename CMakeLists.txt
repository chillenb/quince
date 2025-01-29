cmake_minimum_required(VERSION 3.21)

project(quince LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/fast_float)

option(QUINCE_USE_SYSTEM_PEGTL "Use system PEGTL" OFF)
if(QUINCE_USE_SYSTEM_PEGTL)
  find_package(pegtl REQUIRED)
else()
  add_subdirectory(external/PEGTL)
endif()

add_subdirectory(src)

option(QUINCE_BUILD_TESTS "Build tests" ON)
if(QUINCE_BUILD_TESTS)
  enable_testing()
  add_subdirectory(src/test)
endif()

option(QUINCE_USE_QCINT "Use qcint rather than libcint" ON)

if(QUINCE_USE_QCINT)
  add_subdirectory(external/qcint)
else()
  add_subdirectory(external/libcint)
endif()
