
#include <string>
#include <stdexcept>
#include <iterator>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <boost/regex.hpp>

#include "cetlib/exception.h"
#include "cetlib/split.h"
#include "cetlib/search_path.h"
#include "cetlib/find_matching_files.h"

using namespace std;
using namespace cet;

typedef std::vector<std::string> Strings;

/*
	example regular expression = "^lib([A-Za-z0-9_]+)_plugin.so$"
	
	This program needs to be driven by a shell script to make 
	management of files easier, such as creating and destroying
	temporary directories and files.
*/

int main(int argc, char* argv[])
{
	if(argv==1) { test1(); return 0; }

	if(argc < 3)
	{
		cerr << "usage: " << argv[0] << " regex_pattern directory_name\n";
		return -1;
	}

	Strings files_out;
	find_matching_files(argv[1],argv[2],std::back_inserter(files_out));

	std::copy(files_out.begin(),files_out.end(),
		std::ostream_iterator(cout,"\n"));

	return 0;
}
