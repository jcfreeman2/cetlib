#ifndef cetlib_tokenize_h
#define cetlib_tokenize_h

#include <algorithm>
#include <regex>

namespace cet {

  inline
  std::vector<std::string> tokenize (std::string const& str,
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
