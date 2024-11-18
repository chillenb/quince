cmake_minimum_required(VERSION 3.21)

project(saneqc LANGUAGES C CXX)

find_package(Boost COMPONENTS program_options REQUIRED)
add_executable(saneqc)
add_subdirectory(src)
