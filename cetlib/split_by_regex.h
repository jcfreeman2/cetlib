#ifndef cetlib_tokenize_h
#define cetlib_tokenize_h

// ======================================================================
//
// split_by_regex: Obtain substrings at a string's specified boundaries,
//                 defined according to a regex delimiter set.
//
//                 This is a high-powered method of tokenizing that uses
//                 the C++ regex library to split into substrings.
//
// Examples of splitting with ":" as the separator:
//    input     result
//     ""       [""]         # Different behavior than split.h
//     "a"      ["a"]
//     "a:"     ["a"]
//     ":boo"   ["","boo"]   # Different behavior than split.h
//     "a:b"    ["a","b"]
//     "a::b"   ["a","","b"] # Different behavior than split.h
//
// More complicated example splitting by "::" and "().":
//
//    split_by_regex( "namespace::class::static_function().value", "(::|\\(\\)\\.)" )
//
// returns ["namespace","class","static_function","value"]
//
// ======================================================================

#include <algorithm>
#include <regex>

namespace cet {

  inline
  std::vector<std::string> split_by_regex (std::string const& str,
                                           std::string const& delimSet )
  {
    std::vector<std::string> tokens;
    std::copy( std::sregex_token_iterator(str.begin(),
                                          str.end(),
                                          std::regex( delimSet ),
                                          -1),
               std::sregex_token_iterator(),
               std::back_inserter( tokens ) );
    return tokens;
  }

}

#endif

// Local variables:
// mode: c++
// End:
