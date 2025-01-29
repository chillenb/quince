#include <cstdio>
#include <fstream>
#include <string>

#include <quince/geom.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s filename\n", argv[0]);
    return 1;
  }
  std::string filename(argv[1]);
  std::ifstream in(filename);
  if (!in) {
    printf("Could not open %s\n", filename.c_str());
    return 1;
  }
  XYZFile xyz(in);
  printf("Number of atoms: %zu\n", xyz.natm);
  for (size_t i = 0; i < xyz.natm; ++i) {
    printf("%s %10.5f %10.5f %10.5f\n", xyz.atoms[i].label.c_str(), xyz.atoms[i].x(),
           xyz.atoms[i].y(), xyz.atoms[i].z());
  }
}
