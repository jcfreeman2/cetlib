#include "cetlib/LibraryManager.h"

#include "cetlib/exception.h"
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"
#include "cetlib/demangle.h"
#include "cetlib/search_path.h"

extern "C" {
#include <dlfcn.h>
}

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <functional>
#include <ostream>
#include <sstream>
#include <vector>

cet::LibraryManager::LibraryManager(std::string const & lib_type,
                                    std::string pattern)
  :
  lib_type_(lib_type),
  lib_loc_map_(),
  spec_trans_map_(),
  good_spec_trans_map_(),
  lib_ptr_map_()
{
  using std::placeholders::_1;

  // TODO: We could also consider searching the ld.so.conf list, if
  // anyone asks for it.
  static search_path const
    ld_lib_path(
#if __APPLE__ && __MACH__
      "DY"
#endif
      "LD_LIBRARY_PATH");
  std::vector<std::string> matches;
  ld_lib_path.find_files(shlib_prefix() + pattern +
                         lib_type + dllExtPattern(),
                         matches);
  // Note the use of reverse iterators here: files found earlier in the
  // vector will therefore overwrite those found later, which is what
  // we want from "search path"-type behavior.
  std::for_each(matches.rbegin(), matches.rend(),
                std::bind(&LibraryManager::lib_loc_map_inserter,
                          this,
                          _1));
  // Build the spec to long library name translation table.
  std::for_each(lib_loc_map_.begin(), lib_loc_map_.end(),
                std::bind(&LibraryManager::spec_trans_map_inserter,
                          this,
                          _1,
                          std::ref(lib_type)));
  // Build the fast good-translation table.
  std::for_each(spec_trans_map_.begin(), spec_trans_map_.end(),
                std::bind(&LibraryManager::good_spec_trans_map_inserter,
                          this,
                          _1));
}

size_t
cet::LibraryManager::getLoadableLibraries(std::vector<std::string> &list)
const
{
  return this->getLoadableLibraries(std::back_inserter(list));
}

size_t
cet::LibraryManager::getLoadedLibraries(std::vector<std::string> &list) const
{
  return this->getLoadedLibraries(std::back_inserter(list));
}

size_t
cet::LibraryManager::getValidLibspecs(std::vector<std::string> &list) const
{
  return this->getValidLibspecs(std::back_inserter(list));
}

std::pair<std::string,std::string>
cet::LibraryManager::getSpecsByPath(std::string const & lib_loc) const
{
  std::string short_spec, full_spec;
  for ( const auto & entry : spec_trans_map_ ) {

    std::string const spec            = entry.first;
    std::set<std::string> const paths = entry.second;

    auto const path_iter = paths.find(lib_loc);
    if ( path_iter != paths.end() ) {
      if ( spec.find("/") != std::string::npos ) full_spec = spec;
      else short_spec = spec;
    }
    if ( !short_spec.empty() && !full_spec.empty() ) break;
  }

  if ( short_spec.empty() || full_spec.empty() )
    throw exception("LogicError") << "Missing both short and full specs corresponding to library path.";

  return std::make_pair( short_spec, full_spec );
}

void
cet::LibraryManager::loadAllLibraries() const
{
  for (lib_loc_map_t::const_iterator
       i = lib_loc_map_.begin(),
       end_iter = lib_loc_map_.end();
       i != end_iter;
       ++i) {
    if (get_lib_ptr(i->second) == nullptr) {
      throw exception("Configuration")
          << "Unable to load requested library "
          << i->second
          << "\n"
          << demangle_message(dlerror())
          << "\n";
    }
  }
}

bool
cet::LibraryManager::libraryIsLoaded(std::string const & path) const
{
  return (lib_ptr_map_.find(path) != lib_ptr_map_.end());
}

bool
cet::LibraryManager::libraryIsLoadable(std::string const & path) const
{
  // TODO: If called with any frequency, this should be made more
  // efficient.
  lib_loc_map_t::const_iterator
  i = lib_loc_map_.begin(),
  end_iter = lib_loc_map_.end();
  for (;
       i != end_iter;
       ++i) {
    if (path == i->second) { return true; }
  }
  return false;
}

void
cet::LibraryManager::
lib_loc_map_inserter(std::string const & path)
{
#if BOOST_FILESYSTEM_VERSION == 2
  lib_loc_map_[boost::filesystem::path(path).filename()] = path;
#elif BOOST_FILESYSTEM_VERSION == 3
  lib_loc_map_[boost::filesystem::path(path).filename().native()] = path;
#else
#error unknown BOOST_FILESYSTEM_VERSION!
#endif
}

void
cet::LibraryManager::
spec_trans_map_inserter(lib_loc_map_t::value_type const & entry,
                        std::string const & lib_type)
{

  // First obtain short spec.
  boost::regex e("([^_]+)_" + lib_type + dllExtPattern() + '$');
  boost::match_results<std::string::const_iterator> match_results;
  if (boost::regex_search(entry.first, match_results, e)) {
    spec_trans_map_[match_results[1]].insert(entry.second);
  }
  else {
    throw exception("LogicError")
        << "Internal error in spec_trans_map_inserter for entry "
        << entry.first
        << " with pattern "
        <<  e.str();
  }
  // Next, convert library filename to full libspec.
  std::ostringstream lib_name;
  std::ostream_iterator<char, char> oi(lib_name);
  boost::regex_replace(oi, entry.first.begin(),
                       entry.first.end(),
                       boost::regex("(_+)"),
                       std::string("(?1/)"),
                       boost::match_default | boost::format_all);
  boost::regex stripper("^lib(.*)/" + lib_type + "\\..*$");
  std::string lib_name_str = lib_name.str();
  if (boost::regex_search(lib_name_str, match_results, stripper)) {
    spec_trans_map_[match_results[1]].insert(entry.second);
  }
  else {
    throw exception("LogicError")
        << "Internal error in spec_trans_map_inserter stripping "
        << lib_name.str();
  }
}

void
cet::LibraryManager::
good_spec_trans_map_inserter(spec_trans_map_t::value_type const & entry)
{
  if (entry.second.size() == 1) {
    good_spec_trans_map_[entry.first] = *(entry.second.begin());
  }
}

void * cet::LibraryManager::get_lib_ptr(std::string const & lib_loc) const
{
  lib_ptr_map_t::iterator it = lib_ptr_map_.find(lib_loc);
  if (it == lib_ptr_map_.end() || it->second == nullptr) {
    dlerror();
    void * ptr = dlopen(lib_loc.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    lib_ptr_map_[lib_loc] = ptr;
    return ptr;
  }
  return it->second;
}

void *
cet::LibraryManager::
getSymbolByLibspec_(std::string const & libspec,
                    std::string const & sym_name,
                    bool should_throw_on_dlsym) const
{
  std::string lib_name_str;
  if (libspec.find("_") != std::string::npos) {
    // Plugin names (and hence the class name) cannot contain an underscore.
    throw exception("LogicError", "IllegalUnderscore.")
        << "Library specification \""
        << libspec << "\" contains an illegal underscore.\n"
        << "The class name and path to it may not contain an underscore. "
        << "If this is a configuration error, plase correct it; "
        << "if the module's class name or its location within its "
        << "enclosing package really do have an underscore this situation "
        << "must be rectified.\n";
  }
  good_spec_trans_map_t::const_iterator trans =
    good_spec_trans_map_.find(libspec);
  if (trans == good_spec_trans_map_.end()) {
    // No good translation => zero or too many
    std::ostringstream error_msg;
    error_msg
        << "Library specificaton \""
        << libspec << "\":";
    spec_trans_map_t::const_iterator bad_trans =
      spec_trans_map_.find(libspec);
    if (bad_trans != spec_trans_map_.end()) {
      error_msg << " corresponds to multiple libraries:\n";
      std::copy(bad_trans->second.begin(),
                bad_trans->second.end(),
                std::ostream_iterator<std::string>(error_msg, "\n"));
    }
    else {
      error_msg << " does not correspond to any library in " <<
#if __APPLE__ && __MACH__
        "DY"
#endif
        "LD_LIBRARY_PATH of type \""
                << lib_type_
                << "\"\n";
    }
    throw exception("Configuration")
        << error_msg.str();
  }
  return getSymbolByPath_(trans->second, sym_name, should_throw_on_dlsym);
}

void *
cet::LibraryManager::
getSymbolByPath_(std::string const & lib_loc,
                 std::string const & sym_name,
                 bool should_throw_on_dlsym) const
{
  void * result = nullptr;
  void * lib_ptr = get_lib_ptr(lib_loc);
  if (lib_ptr == nullptr) {
    throw exception("Configuration")
        << "Unable to load requested library "
        << lib_loc
        << "\n"
        << demangle_message(dlerror()) << "\n";
  }
  else { // Found library
    dlerror();
    result = dlsym(lib_ptr, sym_name.c_str());
    char const * error = dlerror();
    if (error != nullptr) { // Error message
      result = nullptr;
      if (should_throw_on_dlsym) {
        throw exception("Configuration")
          << "Unable to load requested symbol "
          <<  demangle_symbol(sym_name)
          << " from library "
          << lib_loc
          << "\n"
          << demangle_message(error)
          << "\n";
      }
    }
  }
  return result;
}

// Local Variables:
// mode: c++
// End:
