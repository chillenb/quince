cmake_minimum_required(VERSION 3.21)

add_executable(testgbs)
target_sources(testgbs PRIVATE gbs.cc)
target_link_libraries(testgbs PRIVATE fast_float taocpp::pegtl)
target_include_directories(testgbs PRIVATE ${PROJECT_SOURCE_DIR}/include)

add_executable(testxyz)
target_sources(testxyz PRIVATE xyz.cc)
target_link_libraries(testxyz PRIVATE fast_float taocpp::pegtl)
target_include_directories(testxyz PRIVATE ${PROJECT_SOURCE_DIR}/include)


add_test(NAME GBSParse COMMAND testgbs ${PROJECT_SOURCE_DIR}/basis/6-31g-short.gbs)
add_test(NAME XYZParse COMMAND testxyz ${PROJECT_SOURCE_DIR}/test/data/h2o.xyz)