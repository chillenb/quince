#include <iostream>
#include <stdexcept>
#include <string>
#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/uri.hpp>
#include <tao/pegtl/file_input.hpp>

#include "fast_float/fast_float.h"

namespace pegtl = TAO_PEGTL_NAMESPACE;

struct GBShellRaw {
  std::string itype;
  int ngauss;
  double scalefactor;
  std::vector<std::vector<double>> block;
};

struct GBBlockRaw {
  std::vector<std::string> centers;
  std::vector<GBShellRaw> shells;
};

struct GBBlockParserState {
  std::vector<GBBlockRaw> blocks;
  GBBlockRaw cur_block;
  GBShellRaw cur_shell;
  std::vector<std::string> cur_centers;
  std::vector<double> cur_line;
};

static double parsedbl(const char *begin, const char *end) {
  fast_float::parse_options options{fast_float::chars_format::fortran};
  double v;
  auto answer = fast_float::from_chars_advanced(begin, end, v, options);
  if (answer.ec != std::errc() || answer.ptr != end) {
    throw std::invalid_argument("could not parse the number " +
                                std::string(begin, end));
  }
  return v;
}

static int parsedbls(const char *begin, const char *end, std::vector<double> &vec) {
  fast_float::parse_options options{fast_float::chars_format::fortran};
  double v;
  int n = 0;
  while (begin != end) {
    auto answer = fast_float::from_chars_advanced(begin, end, v, options);
    if (answer.ec != std::errc() || answer.ptr == begin) {
      throw std::invalid_argument("could not parse the number " +
                                  std::string(begin, end));
    }
    vec.push_back(v);
    begin = answer.ptr;
    n++;
  }
  return n;
}

namespace fdouble_ {
using namespace TAO_PEGTL_NAMESPACE;
struct plus_minus : opt<one<'+', '-'>> {};
struct dot : one<'.'> {};
struct inf : seq<istring<'i', 'n', 'f'>, opt<istring<'i', 'n', 'i', 't', 'y'>>> {};
struct nan : seq<istring<'n', 'a', 'n'>, opt<one<'('>, plus<alnum>, one<')'>>> {};

template <typename D>
struct number : if_then_else<dot, plus<D>, seq<plus<D>, opt<dot, star<D>>>> {};
struct e : one<'e', 'E', 'd', 'D'> {};
struct p : one<'p', 'P'> {};
struct exponent : seq<plus_minus, plus<digit>> {};
struct decimal : seq<number<digit>, opt<e, exponent>> {};
struct grammar : seq<plus_minus, sor<decimal, inf, nan>> {};
} // namespace fdouble_

namespace gbsgrammar {
using namespace TAO_PEGTL_NAMESPACE;
struct ws : ascii::blank {};
struct comment_line : seq<one<'!'>, until<eol>> {};
struct c_nl : sor<comment_line, eol> {};
struct c_wsp : sor<ws, seq<c_nl, ws>> {};
struct block_center : plus<alpha> {};
struct block_header
    : seq<list<block_center, plus<ws>>, star<ws>, one<'0'>, star<ws>, c_nl> {};

struct shell_type : plus<upper> {};
struct shell_ngauss : plus<digit> {};
struct shell_scale_factor : fdouble_::grammar {};

struct shell_header : seq<shell_type, star<ws>, shell_ngauss, star<ws>,
                          shell_scale_factor, star<ws>, c_nl> {};
struct shell_line_entry : fdouble_::grammar {};
struct shell_line_inner : list<shell_line_entry, plus<ws>> {};
struct shell_line : seq<star<ws>, shell_line_inner> {};
struct shell_block : seq<shell_header, star<seq<shell_line, star<c_nl>>>> {};

struct block_terminate : seq<rep<4, one<'*'>>, c_nl> {};
struct block : seq<block_header, plus<shell_block>, block_terminate> {};

struct grammar : seq<star<c_nl>, star<block>, any> {};
template <typename Rule> struct action {};

template <> struct action<block_center> {
  template <typename Input>
  static void apply(const Input &in, GBBlockParserState &state) {
    state.cur_centers.push_back(in.string());
  }
};

template <> struct action<block_header> {
  template <typename Input>
  static void apply(const Input &in, GBBlockParserState &state) {
    state.cur_block.centers = std::move(state.cur_centers);
  }
};

template <> struct action<shell_header> {
  template <typename Input>
  static void apply(const Input &in, GBBlockParserState &state) {}
};

template <> struct action<shell_type> {
  template <typename Input>
  static void apply(const Input &in, GBBlockParserState &state) {
    state.cur_shell.itype = in.string();
  }
};

template <> struct action<shell_ngauss> {
  template <typename Input>
  static void apply(const Input &in, GBBlockParserState &state) {
    state.cur_shell.ngauss = std::stoi(in.string());
  }
};

template <> struct action<shell_scale_factor> {
  template <typename Input>
  static void apply(const Input &in, GBBlockParserState &state) {
    state.cur_shell.scalefactor = parsedbl(in.begin(), in.end());
  }
};

template <> struct action<shell_line_entry> {
  template <typename Input>
  static void apply(const Input &in, GBBlockParserState &state) {
    state.cur_line.push_back(parsedbl(in.begin(), in.end()));
  }
};

template <> struct action<shell_line> {
  template <typename Input>
  static void apply(const Input &in, GBBlockParserState &state) {
    state.cur_shell.block.push_back(std::move(state.cur_line));
  }
};

template <> struct action<shell_block> {
  template <typename Input>
  static void apply(const Input &in, GBBlockParserState &state) {
    state.cur_block.shells.push_back(std::move(state.cur_shell));
  }
};

template <> struct action<block> {
  template <typename Input>
  static void apply(const Input &in, GBBlockParserState &state) {
    state.blocks.push_back(std::move(state.cur_block));
  }
};

} // namespace gbsgrammar

template <typename Input> static std::vector<GBBlockRaw> parse_gbs(Input &in) {
  GBBlockParserState blk;
  try {
    pegtl::parse<gbsgrammar::grammar, gbsgrammar::action>(in, blk);
  } catch (const TAO_PEGTL_NAMESPACE::parse_error &e) {
    const auto &p = e.positions().front();
    std::cerr << e.what() << std::endl
              << in.line_at(p) << '\n'
              << std::setw(p.column) << '^' << std::endl;
  }
  return std::move(blk.blocks);
}
