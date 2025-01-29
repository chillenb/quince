#include <iostream>
#include <quince/geom.h>
#include <quince/parse/fdouble.h>
#include <string>
#include <tao/pegtl.hpp>
#include <tao/pegtl/file_input.hpp>

namespace pegtl = TAO_PEGTL_NAMESPACE;

struct XYZParserState {
  XYZFile xyz;
  std::string cur_label;
  std::vector<double> cur_coords;
};

namespace xyzgrammar {
using namespace TAO_PEGTL_NAMESPACE;
struct ws : ascii::blank {};
struct c_nl : eol {};

struct natm : plus<digit> {};
struct line1 : must<star<ws>, natm, star<ws>, c_nl> {};
struct line2comment : until<at<c_nl>> {};
struct line2 : seq<line2comment, c_nl> {};

struct coord : fdouble_::grammar {};
struct label : plus<alnum> {};
struct entry
    : seq<label, plus<ws>, coord, plus<ws>, coord, plus<ws>, coord, star<ws>, c_nl> {
};

struct grammar : must<line1, line2, plus<entry>> {};
template <typename Rule> struct action {};

template <> struct action<natm> {
  template <typename Input>
  static void apply(const Input &in, XYZParserState &state) {
    state.xyz.natm = std::stoi(in.string());
  }
};

template <> struct action<line2comment> {
  template <typename Input>
  static void apply(const Input &in, XYZParserState &state) {
    state.xyz.comment = in.string();
  }
};

template <> struct action<coord> {
  template <typename Input>
  static void apply(const Input &in, XYZParserState &state) {
    state.cur_coords.push_back(parsedbl(in.begin(), in.end()));
  }
};

template <> struct action<label> {
  template <typename Input>
  static void apply(const Input &in, XYZParserState &state) {
    state.cur_label = in.string();
  }
};

template <> struct action<entry> {
  template <typename Input>
  static void apply(const Input &in, XYZParserState &state) {
    state.xyz.atoms.emplace_back(state.cur_label, state.cur_coords[0],
                                 state.cur_coords[1], state.cur_coords[2]);
    state.cur_coords.clear();
  }
};

} // namespace xyzgrammar

template <typename Input> static XYZFile parse_xyz(Input &in) {
  XYZParserState xyzp;
  try {
    pegtl::parse<xyzgrammar::grammar, xyzgrammar::action>(in, xyzp);
  } catch (const TAO_PEGTL_NAMESPACE::parse_error &e) {
    const auto &p = e.positions().front();
    std::cerr << e.what() << std::endl
              << in.line_at(p) << '\n'
              << std::setw(p.column) << '^' << std::endl;
  }
  return std::move(xyzp.xyz);
}
