#ifndef QUINCE_GEOM_H
#define QUINCE_GEOM_H

#include <istream>
#include <stddef.h>
#include <string>
#include <vector>

struct XYZEntry {
  // label can be atomic number, element symbol, or element with a unique
  // integer identifier.
  std::string label;
  double coords[3];
  double &x() { return coords[0]; }
  double &y() { return coords[1]; }
  double &z() { return coords[2]; }
  XYZEntry() {};
  XYZEntry(const std::string label, double x, double y, double z) : label(label) {
    coords[0] = x;
    coords[1] = y;
    coords[2] = z;
  }
};

struct XYZFile {
  size_t natm;
  std::string comment;
  std::vector<XYZEntry> atoms;
  XYZFile() {};
  XYZFile(std::istream &in);
};

#endif // QUINCE_GEOM_H