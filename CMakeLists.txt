cmake_minimum_required(VERSION 3.21)

project(saneqc LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(
  fast_float
  GIT_REPOSITORY https://github.com/fastfloat/fast_float.git
  GIT_TAG tags/v6.1.6
  GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(fast_float)

find_package(pegtl REQUIRED)

add_executable(saneqc)
add_executable(parsegbs)
add_subdirectory(src)
