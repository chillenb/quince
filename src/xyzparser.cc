#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/fusion.hpp>
#include <boost/phoenix/stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "geom.h"

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

BOOST_FUSION_ADAPT_STRUCT(
    XYZEntry,
    (std::string, label)
    (double[3], coords)
)

template <typename Iterator>
struct XYZParserInner : qi::grammar<Iterator, XYZEntry(), ascii::space_type>
{
    XYZParserInner() : XYZParserInner::base_type(start)
    {
        using qi::int_;
        using qi::lit;
        using qi::double_;
        using qi::lexeme;
        using ascii::char_;

        start %= lexeme[+char_("A-Za-z")] >> double_ >> double_ >> double_;
    }

    qi::rule<Iterator, XYZEntry(), ascii::space_type> start;
};

template <typename Iterator>
struct XYZParser : qi::grammar<Iterator, XYZFile(), ascii::space_type>
{
    XYZParser() : XYZParser::base_type(start)
    {
        using qi::int_;
        using qi::lit;
        using qi::double_;
        using qi::lexeme;
        using ascii::char_;

        start %= int_ >> lit('\n') >> *(inner);
        inner %= lexeme[+char_("A-Za-z")] >> double_ >> double_ >> double_ >> lit('\n');
    }

    XYZParserInner<Iterator> inner;
    qi::rule<Iterator, XYZFile(), ascii::space_type> start;
    qi::rule<Iterator, XYZEntry(), ascii::space_type> inner;
};
