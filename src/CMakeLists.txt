
target_sources(saneqc PRIVATE main.cc xyzparser.cc)
target_sources(parsegbs PRIVATE gbs.cc)
target_link_libraries(saneqc PRIVATE fast_float)
target_link_libraries(parsegbs PRIVATE fast_float taocpp::pegtl)