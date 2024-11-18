
target_sources(saneqc PRIVATE main.cc xyzparser.cc)
target_link_libraries(saneqc PUBLIC Boost::program_options)