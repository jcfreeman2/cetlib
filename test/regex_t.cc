#ifdef STANDALONE_TEST
#include <cassert>
#define BOOST_REQUIRE assert
#define BOOST_CHECK assert
#else
#define BOOST_TEST_MODULE ( regex test )
#include "boost/test/auto_unit_test.hpp"
#include "cetlib/LibraryManager.h"
#endif

#include <fstream>
#include <iostream>
#include <regex>

#ifdef STANDALONE_TEST
int main()
#else
BOOST_AUTO_TEST_SUITE(RegexTests)

BOOST_AUTO_TEST_CASE(pluginRegex)
#endif
{
  std::ifstream inFile("regex.txt");
  BOOST_REQUIRE(inFile);
  std::string line;
#ifdef STANDALONE_TEST
  std::string const pattern_stem = { "(?:[A-Za-z0-9\\-]*_)*[A-Za-z0-9]+_" };
#else
  cet::LibraryManager dummy("noplugin");
  std::string const pattern_stem = dummy.patternStem();
#endif
  std::string pattern = std::string("lib(") + pattern_stem + ")([A-Za-z0-9]+)\\.so";
  while (std::getline(inFile, line)) {
    static std::regex const r(pattern);
    std::smatch subs;
    bool result = std::regex_match(line, subs, r);
    std::cout << line << " match: " << std::boolalpha << result
              << (result ? (std::string(" (stem ") + subs[1].str().substr(0, subs[1].str().size() - 1) + ", type " + subs[2].str() + ")") : "") << std::endl;
    BOOST_CHECK(result);
  }
}

#ifndef STANDALONE_TEST
BOOST_AUTO_TEST_SUITE_END()
#endif
