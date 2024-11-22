cmake_minimum_required(VERSION 3.21)

project(saneqc LANGUAGES C CXX)

include(FetchContent)
FetchContent_Declare(
  fast_float
  GIT_REPOSITORY https://github.com/fastfloat/fast_float.git
  GIT_TAG tags/v6.1.6
  GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(fast_float)

include(FetchContent)
FetchContent_Declare(lexy URL https://lexy.foonathan.net/download/lexy-src.zip)
FetchContent_MakeAvailable(lexy)

add_executable(saneqc)
add_subdirectory(src)
