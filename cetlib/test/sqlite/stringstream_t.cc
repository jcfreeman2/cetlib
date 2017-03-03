#include "cetlib/sqlite/Exception.h"
#include "cetlib/sqlite/stringstream.h"

#include <string>

using namespace std::string_literals;
using namespace cet::sqlite;

int main() {

  stringstream ss;

  // put a few things in
  ss << "const char*"
     << "std::string"s
     << 1
     << 2.4
     << 3.6f;

  std::string first;
  std::string second;
  int one;
  double next;
  float last;

  if (!ss.empty()) {
    ss >> first
       >> second
       >> one
       >> next
       >> last;
  }

  try {
    int x;
    ss >> x;
    throw Exception(errors::Unknown,"shouldn't get here");
  }
  catch (Exception const& e) {
    if (e.categoryCode() == errors::Unknown) {
      throw Exception(errors::Unknown, "this is really bad");
    }
  }
  catch (...){}

}
