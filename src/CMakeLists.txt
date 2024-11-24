cmake_minimum_required(VERSION 3.21)

add_executable(saneqc)
target_sources(saneqc PRIVATE main.cc xyzparser.cc)
set_target_properties(saneqc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

target_link_libraries(saneqc PRIVATE fast_float)
target_include_directories(saneqc PRIVATE ${PROJECT_SOURCE_DIR}/include)