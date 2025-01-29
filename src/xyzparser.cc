#include "fast_float/fast_float.h"
#include <cstdlib>
#include <iostream>
#include <istream>
#include <quince/geom.h>

XYZFile::XYZFile(std::istream &in) {
  fast_float::parse_options options{fast_float::chars_format::fortran};

  in >> natm;
  atoms.resize(natm);
  std::string dbl;
  for (size_t i = 0; i < natm; ++i) {
    in >> atoms[i].label;
    for (int j = 0; j < 3; j++) {
      in >> dbl;
      auto answer = fast_float::from_chars_advanced(
          dbl.data(), dbl.data() + dbl.size(), atoms[i].coords[j], options);
      if (answer.ec != std::errc() || answer.ptr != dbl.data() + dbl.size()) {
        std::cerr << "could not parse the number " << dbl << " for atom " << i
                  << " coordinate " << j + 1 << "\n";
        exit(-1);
      }
    }
  }
}