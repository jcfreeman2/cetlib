#include "cetlib/filepath_maker.h"
#include "cetlib/includer.h"
//#include <cstdlib>
#include <fstream>

void
  ensure( int which, bool claim )
{
  if( not claim )
    std::exit(which);
}

char const file_a[] = "./a.txt";
char const file_b[] = "./b.txt";
char const file_i[] = "./i.txt";
char const file_j[] = "./j.txt";

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

void
  write_files( )
{
  std::ofstream a(file_a);  a << contents_a;
  std::ofstream b(file_b);  b << contents_b;
  std::ofstream i(file_i);  i << contents_i;
  std::ofstream j(file_j);  j << contents_j;
}


int
  main( )
{
  write_files();
  cet::filepath_lookup policy(".:./test");

  { cet::includer a(file_a, policy);
    std::string result1(a.begin(), a.end());
    ensure( 1, result1 == std::string(contents_a) );

    cet::includer b(file_b, policy);
    std::string result2(b.begin(), b.end());
    ensure( 2, result2 == std::string(contents_b) );
  }

  { cet::includer i(file_i, policy);
    std::string result(i.begin(), i.end());
    ensure( 11, result == std::string("begin\n")
                        + std::string(contents_a)
                        + std::string("end\n")
          );
  }

  { cet::includer j(file_j, policy);
    std::string result(j.begin(), j.end());
    ensure( 12, result == std::string("begin\n")
                        + std::string(contents_a)
                        + std::string(contents_b)
                        + std::string("end\n")
          );
  }

  return 0;

}  // main()
