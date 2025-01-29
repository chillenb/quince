#include <cstdio>
#include <quince/parse/xyz.h>
#include <tao/pegtl.hpp>

int main(int argc, char *argv[]) {
  if (argc != 2)
    return 1;

  pegtl::file_input in(argv[1]);

  auto xyz = parse_xyz(in);
  printf("Number of atoms: %zu\n", xyz.natm);
  printf("Comment: %s\n", xyz.comment.c_str());
  for (int i = 0; i < xyz.natm; ++i) {
    auto a = xyz.atoms[i];
    printf("%s %10.5f %10.5f %10.5f\n", a.label.c_str(), a.x(), a.y(), a.z());
  }

  return 0;
}
