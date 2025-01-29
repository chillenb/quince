cmake_minimum_required(VERSION 3.21)

add_executable(quince)
target_sources(quince PRIVATE main.cc xyzparser.cc)
set_target_properties(quince PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

target_link_libraries(quince PRIVATE fast_float)
target_include_directories(quince PRIVATE ${PROJECT_SOURCE_DIR}/include)