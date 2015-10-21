#ifndef CETLIB_REPLACE_ALL_H
#define CETLIB_REPLACE_ALL_H

#include <string>

namespace cet {
  ///
  /// \brief Replace all occurrences of from in string with to.
  ///
  /// \returns true if anything has been done to in.
  ///
  /// \param[in,out] in The string to be manipulated.
  /// \param[in] from The string to be replaced.
  /// \param[in] to The replacement of from.
  bool
  replace_all(std::string & in,
              std::string const & from,
              std::string const & to);
}

#endif /* CETLIB_REPLACE_ALL_H */

// Local variables:
// mode:c++
// End:
