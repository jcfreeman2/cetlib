#include "cetlib/include.h"
#include <cstdlib>
#include <fstream>


using cet::include;


void
ensure( int which, bool claim )
{
  if( not claim )
    std::exit(which);
}

inline
void
ensure(int which, std::istream &os)
{
  // Force boolean context.
  ensure(which, !!os);
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
                          "#include \"./a.txt\"\n"
                          "end\n";
char const contents_j[] = "begin\n"
                          "#include \"./a.txt\"\n"
                          "#include \"./b.txt\"\r\n"
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

  { std::string result;

    std::ifstream a(file_a);
    ensure( 1, a );
    result.clear();
    include(a, result);
    ensure( 2, result == std::string(contents_a) );

    std::ifstream b(file_b);
    ensure( 3, b );
    result.clear();
    include(b, result);
    ensure( 4, result == std::string(contents_b) );
  }

  { std::string result;
    std::ifstream i(file_i);
    ensure( 11, i );
    result.clear();
    include(i, result);
    ensure( 12, result == std::string("begin\n")
                        + std::string(contents_a)
                        + std::string("end\n")
          );
  }

  { std::string result;
    std::ifstream j(file_j);
    ensure( 21, j );
    result.clear();
    include(j, result);
    ensure( 12, result == std::string("begin\n")
                        + std::string(contents_a)
                        + std::string(contents_b)
                        + std::string("end\n")
          );
  }

  return 0;

}  // main()
