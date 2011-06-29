// ======================================================================
//
// includer_test
//
// ======================================================================

#define BOOST_TEST_MODULE ( includer test )
#include "boost/test/auto_unit_test.hpp"

#include "cetlib/exception.h"
#include "cetlib/filepath_maker.h"
#include "cetlib/includer.h"
#include <fstream>

char const file_a[] = "./a.txt";
char const file_b[] = "./b.txt";
char const file_i[] = "./i.txt";
char const file_j[] = "./j.txt";
char const file_r[] = "./r.txt";

char const contents_a[] = "abcde\n"
                          "vwxyz\n";
char const contents_b[] = "12345\n"
                          "67890\n";
char const contents_i[] = "begin\n"
                          "#include \"./a.txt\" \t  \n"
                          "end\n";
char const contents_j[] = "begin\n"
                          "#include \"./a.txt\"\n"
                          "#include \"./b.txt\"\t\n"
                          "end\n";
char const contents_r[] = "begin\n"
                          "#include \"./r.txt\"\n"
                          "end\n";

BOOST_AUTO_TEST_SUITE( includer_test )

void
  write_files( )
{
  std::ofstream a(file_a);  a << contents_a;
  std::ofstream b(file_b);  b << contents_b;
  std::ofstream i(file_i);  i << contents_i;
  std::ofstream j(file_j);  j << contents_j;
  std::ofstream r(file_r);  r << contents_r;
}


cet::filepath_lookup policy(".:./test");


BOOST_AUTO_TEST_CASE( no_inclusion_test )
{
  write_files();

  cet::includer a(file_a, policy);
  std::string result1(a.begin(), a.end());
  BOOST_CHECK_EQUAL( result1, std::string(contents_a) );

  cet::includer b(file_b, policy);
  std::string result2(b.begin(), b.end());
  BOOST_CHECK_EQUAL( result2, std::string(contents_b) );
}

BOOST_AUTO_TEST_CASE( single_inclusion_test )
{
  cet::includer i(file_i, policy);
  std::string result(i.begin(), i.end());
  BOOST_CHECK_EQUAL( result,   std::string("begin\n")
                             + std::string(contents_a)
                             + std::string("end\n")
                   );
}

BOOST_AUTO_TEST_CASE( double_inclusion_test )
{
  cet::includer j(file_j, policy);
  std::string result(j.begin(), j.end());
  BOOST_CHECK_EQUAL( result,   std::string("begin\n")
                             + std::string(contents_a)
                             + std::string(contents_b)
                             + std::string("end\n")
                   );
}

BOOST_AUTO_TEST_CASE( recursive_inclusion_test )
{
  BOOST_CHECK_THROW( cet::includer(file_r, policy), cet::exception );

  try  {
    cet::includer r(file_r, policy);
    BOOST_FAIL("Failed to throw an exception as expected");
  }
  catch( cet::exception const & e )  {
    std::string message(e.what());
    BOOST_CHECK( message.find(file_r) != std::string::npos );
  }
  catch( ... )  {
    BOOST_FAIL("Wrong exception type thrown");
  }
}

BOOST_AUTO_TEST_SUITE_END()
