#include <stdexcept>
#include <string>
#include <tao/pegtl.hpp>
#include <tao/pegtl/file_input.hpp>

#include <fast_float/fast_float.h>

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
