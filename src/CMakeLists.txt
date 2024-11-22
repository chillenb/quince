
target_sources(saneqc PRIVATE main.cc xyzparser.cc)
target_link_libraries(saneqc PRIVATE fast_float foonathan::lexy)