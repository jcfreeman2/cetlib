#include "cetlib/detail/plugin_search_path.h"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

extern "C" {
#include <unistd.h>
}

void usage(int ecode = EXIT_FAILURE)
{
  std::cerr <<
    R"(Usage: cet_bootstrap <program> <args>+
       cet_bootstrap -h

Execute the provided program and arguments in an environment where the contents
of CET_PLUGIN_PATH have been injected into (DY)LD_LIBRARY_PATH.
)";
  exit(ecode);
}

int main(int argc, char **argv)
{
  // Check arguments.
  if (argc < 2 || strncmp(argv[1], "-h", 2) == 0) {
    usage();
  }
  // Inject CET_PLUGIN_PATH into dynamic library path.
  (void) cet::detail::plugin_search_path();
  // Execute specified program with arguments in the current
  // environment.
  std::vector<char *> newargs(argv+1, argv + argc);
  newargs.push_back(nullptr); // Required by execvp calling convention.
  execvp(argv[1], newargs.data());
  // If we get here, execvp() has failed.
  std::ostringstream errmsg;
  errmsg << "cet_bootstrap: failure to execute ";
  std::copy(newargs.cbegin(),
            newargs.cbegin() + newargs.size() - 1,
            std::ostream_iterator<char *>(errmsg, " "));
  errmsg << "with error";
  perror(errmsg.str().c_str());
  exit(EXIT_FAILURE);
}
