#include <iostream>
#include <tao/pegtl.hpp>

#include <quince/parse/gbs.h>

int main(int argc, char *argv[]) {
  if (argc != 2)
    return 1;

  pegtl::file_input in(argv[1]);

  auto blk = parse_gbs(in);
  for (const auto &s : blk) {
    for (const auto &c : s.centers) {
      std::cout << c << std::endl;
    }
    for (const auto &s : s.shells) {
      printf("%s %d %3.2f\n", s.itype.c_str(), s.ngauss, s.scalefactor);
      for (const auto &l : s.block) {
        printf("    ");
        for (const auto &e : l) {
          printf("%15E ", e);
        }
        std::cout << std::endl;
      }
    }
  }

  return 0;
}
