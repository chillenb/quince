#include <cstdio>


#include <iostream>
#include <lexy/callback/aggregate.hpp>
#include <lexy/callback/bind.hpp>
#include <string>
#include <vector>
#include <stdexcept>

#include "fast_float/fast_float.h"
#include <lexy/dsl/char_class.hpp>
#include <lexy/encoding.hpp>
#include <lexy/grammar.hpp>
#include <lexy/action/parse.hpp>
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/input/argv_input.hpp>
#include <lexy/input/file.hpp>
#include <lexy_ext/report_error.hpp>

struct GBShellRaw
{
  std::string itype;
  int ngauss;
  double scalefactor;
  std::vector<std::vector<double>> block;
};

struct GBBlockRaw
{
  std::vector<std::string> centers;
  std::vector<GBShellRaw> shells;
};

static constexpr double parsedbl(const char *begin, const char *end) {
  fast_float::parse_options options{fast_float::chars_format::fortran};
  double v;
  auto answer = fast_float::from_chars_advanced(begin, end, v, options);
  if (answer.ec != std::errc() || answer.ptr != end) {
    throw std::invalid_argument("could not parse the number " +
                                std::string(begin, end));
  }
  return v;
}

namespace gbgrammar
{



  namespace dsl = lexy::dsl;
  static constexpr auto ws = dsl::whitespace(dsl::ascii::blank);

  // the valid characters for a floating point number.
  constexpr auto numchar = LEXY_CHAR_CLASS("numchar", dsl::ascii::digit /
                                                  LEXY_LIT(".") /
                                                  LEXY_LIT("-") /
                                                  LEXY_LIT("+") /
                                                  LEXY_LIT("e") /
                                                  LEXY_LIT("E") /
                                                  LEXY_LIT("d") /
                                                  LEXY_LIT("D"));

  // Fortran or C style floating point number.
  struct number : lexy::token_production
  {
    static constexpr auto rule = LEXY_LIT("+") >> dsl::identifier(numchar)
                                  | dsl::identifier(numchar);
    static constexpr auto value = lexy::callback<double>(
        [](auto lex) {
              auto lexstr = lexy::as_string<std::string>(lex);
              return parsedbl(lexstr.data(), lexstr.data() + lexstr.size());
        });
  };

  struct elem_or_shelltype
  {
    static constexpr auto rule = dsl::identifier(dsl::ascii::alpha);
    static constexpr auto value = lexy::as_string<std::string>;
  };

  struct line_done
  {
    static constexpr auto rule = ws + (LEXY_LIT("!") | dsl::peek(dsl::newline));
  };


  struct comment_lines
  {
    static constexpr auto rule = ws + dsl::while_(dsl::peek(LEXY_LIT("!")) >> dsl::until(dsl::newline).or_eof()
                                                 | dsl::peek(dsl::newline) >> dsl::newline);
    static constexpr auto value = lexy::noop;
  };

  struct shell_line
  {
    static constexpr auto rule = dsl::list(dsl::peek_not(dsl::p<line_done>) >> dsl::p<number> + ws);
    static constexpr auto value = lexy::as_list<std::vector<double>>;
  };


  struct shell
  {
    static constexpr auto rule = [] {
      auto itype = dsl::p<elem_or_shelltype>;
      auto ngauss = dsl::integer<int>;
      auto scalefactor = dsl::p<number>;

      auto line1 = itype + ws + ngauss + ws + scalefactor + dsl::p<comment_lines>;
      //auto shell = dsl::list(dsl::break_ >> dsl::p<shell_line> + dsl::p<comment_lines>);


      return dsl::p<comment_lines> + line1;
    }();

    static constexpr auto value = lexy::construct<GBShellRaw>;


  };


  struct block_header
  {
    static constexpr auto rule = dsl::list(dsl::peek_not(LEXY_LIT("0")) >> dsl::identifier(dsl::ascii::alpha) + ws);
    static constexpr auto value = lexy::fold_inplace<std::vector<std::string>>(
        std::vector<std::string>{},
        [](auto &vec, auto lex) {
          vec.push_back(lexy::as_string<std::string>(lex));
        });
  };

  struct block
  {
    static constexpr auto rule = [] {
      auto header = dsl::p<comment_lines> + dsl::p<block_header>;
      //auto rshell = dsl::p<shell>;

      return header + dsl::list(dsl::p<shell>, dsl::sep(LEXY_LIT(""))); //dsl::list(dsl::peek_not("****") >> dsl::p<shell>);
    }();

    static constexpr auto value =  lexy::bind(
      lexy::construct<GBBlockRaw>,
      lexy::_1,
      lexy::bind(lexy::as_list<std::vector<GBShellRaw>>, lexy::_2)
    );
  };



}

int main(int argc, char* argv[])
{
    // Scan the IP address provided at the commandline.
    auto file = lexy::read_file(argv[1]);
    auto result = lexy::parse<gbgrammar::block>(file.buffer(), lexy_ext::report_error);
    if (!result.has_value())
        return 1;

    auto value = result.value();

    // And print it as an integer.
    // for (auto& str : value)
    //     std::cout << str << '\n';
    for(auto &c : value.centers)
      std::cout << c << '\n';
    // for(auto &s : value)
    // {
    //   std::cout << s.itype << " " << s.ngauss << " " << s.scalefactor << '\n';
    //   for(auto &b : s.block)
    //   {
    //     for(auto &v : b)
    //       std::cout << v << " ";
    //     std::cout << '\n';
    //   }
    // }
}