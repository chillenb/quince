#ifndef SANEQC_GEOM_H
#define SANEQC_GEOM_H

#include <stddef.h>
#include <string>
#include <vector>

struct XYZEntry
{
  // label can be atomic number, element symbol, or element with a unique integer identifier.
  std::string label;
  double coords[3];
  double& x() { return coords[0]; }
  double& y() { return coords[1]; }
  double& z() { return coords[2]; }
};

struct XYZFile
{
  size_t natm;
  std::vector<XYZEntry> atoms;
};

#endif // SANEQC_GEOM_H