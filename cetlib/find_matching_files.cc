
#include "cetlib/find_matching_files.h"
#include <boost/regex.hpp>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <dirent.h>

using namespace std;
using namespace boost;

namespace cet
{

  size_t find_matching_files(std::string const& pat
				,std::string const& dir
				,std::vector<std::string>& out)
  {
    DIR* dd = opendir(dir.c_str());
    if(!dd) return 0;
    
    boost::regex e(pat);

    int err=0;
    int count=0;
    struct dirent entry;
    struct dirent* result=0;
    // cmatch what;  // not yet needed
    
    while(!(err=readdir_r(dd,&entry,&result)) && result!=0)
      {
	if(boost::regex_match(entry.d_name,e))
	  {
	    out.push_back(entry.d_name);
	    ++count;
	  }
      }
    
    closedir(dd);
    
    if(result!=0)
      {
	ostringstream ost;
	ost << "Failed to read directory " << dir << ", error num=" << err;
	throw std::runtime_error(ost.str());
      }
    
    return count;
  }

}
