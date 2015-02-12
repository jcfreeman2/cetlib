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
#include <iostream>
#include <sstream>

char const file_a[] = "./a.txt";
char const file_b[] = "./b.txt";
char const file_i[] = "./i.txt";
char const file_j[] = "./j.txt";
char const file_k[] = "./k.txt";
char const file_r[] = "./r.txt";
char const file_r2[] = "./r2.txt";
char const file_r3[] = "./r3.txt";
char const file_r4[] = "./r4.txt";
char const file_x1[] = "./x1.txt";
char const file_x2[] = "./x2.txt";
char const file_x3[] = "./x3.txt";

char const contents_a [] = "abcde\n"
                           "vwxyz\n";
char const contents_b [] = "12345\n"
                           "67890\n";
char const contents_i [] = "begin\n"
                           "#include \"./a.txt\" \t  \n"
                           "end\n";
char const contents_j [] = "begin\n"
                           "#include \"./a.txt\"\n"
                           "#include \"./b.txt\"\t\r\n"
                           "end\n";
char const contents_k [] = "begin\n"
                           "#include \"./j.txt\"\n"
                           "#include \"./i.txt\"\n"
                           "end\n";
char const contents_r [] = "begin\n"
                           "#include \"./r.txt\"\n"
                           "end\n";
char const contents_r2 [] = "begin\n"
                            "#include \"./r3.txt\"\n"
                            "end\n";
char const contents_r3 [] = "begin\n"
                            "#include \"./j.txt\"\n"
                            "#include \"./r4.txt\"\n"
                            "end\n";
char const contents_r4 [] = "begin\n"
                            "#include \"./r2.txt\"\n"
                            "end\n";
char const contents_x1[] = "#include\"./a.txt\"\n";
char const contents_x2[] = "#include ./a.txt\"\n";
char const contents_x3[] = "#include \"./a.txt\n";

BOOST_AUTO_TEST_SUITE( includer_test )

void
  write_files( )
{
  std::ofstream a (file_a );  a  << contents_a;
  std::ofstream b (file_b );  b  << contents_b;
  std::ofstream i (file_i );  i  << contents_i;
  std::ofstream j (file_j );  j  << contents_j;
  std::ofstream k (file_k );  k  << contents_k;
  std::ofstream r (file_r );  r  << contents_r;
  std::ofstream r2 (file_r2 );  r2  << contents_r2;
  std::ofstream r3 (file_r3 );  r3  << contents_r3;
  std::ofstream r4 (file_r4 );  r4  << contents_r4;
  std::ofstream x1(file_x1);  x1 << contents_x1;
  std::ofstream x2(file_x2);  x2 << contents_x2;
  std::ofstream x3(file_x3);  x3 << contents_x3;
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

BOOST_AUTO_TEST_CASE( repeated_inclusion_test )
{
  cet::includer k(file_k, policy);
  std::string result(k.begin(), k.end());
  BOOST_CHECK_EQUAL( result,
                     std::string("begin\n") +
                     std::string("begin\n") +
                     std::string(contents_a) +
                     std::string(contents_b) +
                     std::string("end\n") +
                     std::string("begin\n") +
                     std::string(contents_a) +
                     std::string("end\n") +
                     std::string("end\n")
                   );
}

BOOST_AUTO_TEST_CASE( malformed_inclusion_test )
{
  BOOST_CHECK_THROW( cet::includer(file_x1, policy), cet::exception );
  BOOST_CHECK_THROW( cet::includer(file_x2, policy), cet::exception );
  BOOST_CHECK_THROW( cet::includer(file_x3, policy), cet::exception );
}

BOOST_AUTO_TEST_CASE( simple_recursive_inclusion_test )
{
  BOOST_CHECK_EXCEPTION(cet::includer(file_r, policy), \
                        cet::exception, \
                        [](cet::exception const & e) -> bool \
                        { \
                          return e.category() == "Recursive #include directive:"; \
                        });
}

BOOST_AUTO_TEST_CASE( complex_recursive_inclusion_test )
{
  BOOST_CHECK_EXCEPTION(cet::includer(file_r2, policy), \
                        cet::exception, \
                        [](cet::exception const & e) -> bool \
                        { \
                          return e.category() == "Recursive #include directive:"; \
                        });
}

BOOST_AUTO_TEST_CASE( string_inclusion_test )
{
  std::string a = contents_a;
  std::istringstream is_a(a);
  cet::includer inc_a(is_a, policy);
  std::string result_a(inc_a.begin(), inc_a.end());
  BOOST_CHECK_EQUAL( result_a, a );

  std::string i = contents_i;
  std::istringstream is_i(i);
  cet::includer inc_i(is_i, policy);
  std::string result_i(inc_i.begin(), inc_i.end());
  BOOST_CHECK_EQUAL( result_i,   std::string("begin\n")
                               + std::string(contents_a)
                               + std::string("end\n")
                     );
}

BOOST_AUTO_TEST_CASE( backtrace_test )
{
  cet::includer j(file_j, policy);
  auto it = j.begin();
  std::advance(it, 5);
  BOOST_REQUIRE(*it == '\n');
  std::cerr << j.whereis(it) << "\n";
  std::cerr << "\n";
  std::string cmp("line 1, character 6, of file \"././j.txt\"");
  BOOST_REQUIRE_EQUAL(j.whereis(it), cmp);
  std::advance(it, 10);
  BOOST_REQUIRE(*it == 'y');
  std::cerr << j.whereis(it) << "\n";
  std::cerr << "\n";
  cmp = "line 2, character 4, of file \"././a.txt\"\n"
        "included from line 2 of file \"././j.txt\"";
  BOOST_REQUIRE_EQUAL(j.whereis(it), cmp);
  std::advance(it, 10);
  BOOST_REQUIRE(*it == '7');
  std::cerr << j.whereis(it) << "\n";
  cmp = "line 2, character 2, of file \"././b.txt\"\n"
        "included from line 3 of file \"././j.txt\"";
  BOOST_REQUIRE_EQUAL(j.whereis(it), cmp);
}

BOOST_AUTO_TEST_CASE( highlighted_backtrace_test )
{
  cet::includer j(file_j, policy);
  auto it = j.begin();
  std::advance(it, 5);
  BOOST_REQUIRE(*it == '\n');
  std::cerr << j.highlighted_whereis(it) << "\n";
  std::cerr << "\n";
  std::string cmp("line 1, character 6, of file \"././j.txt\"");
  cmp += "\n\nbegin\n     ^";
  BOOST_REQUIRE_EQUAL(j.highlighted_whereis(it), cmp);
  std::advance(it, 10);
  BOOST_REQUIRE(*it == 'y');
  std::cerr << j.highlighted_whereis(it) << "\n";
  std::cerr << "\n";
  cmp = "line 2, character 4, of file \"././a.txt\"\n"
        "included from line 2 of file \"././j.txt\"";
  cmp += "\n\nvwxyz\n   ^";
  BOOST_REQUIRE_EQUAL(j.highlighted_whereis(it), cmp);
  std::advance(it, 10);
  BOOST_REQUIRE(*it == '7');
  std::cerr << j.highlighted_whereis(it) << "\n";
  cmp = "line 2, character 2, of file \"././b.txt\"\n"
        "included from line 3 of file \"././j.txt\"";
  cmp += "\n\n67890\n ^";
  BOOST_REQUIRE_EQUAL(j.highlighted_whereis(it), cmp);
}

BOOST_AUTO_TEST_SUITE_END()
