#ifndef cetlib_split_h
#define cetlib_split_h

// ======================================================================
//
// split: Obtain substrings at a string's specified boundaries.
//
// Examples of splitting with ':' as the separator:
//    input     result
//     ""       [ ]                # Different behavior than split_by_regex.h
//     "a"      ["a"]
//     "a:"     ["a"]
//     ":boo"   ["boo"]            # Different behavior than split_by_regex.h
//     "a:b"    ["a","b"]
//     "a::b"   ["a","b"] // N.B.! # Different behavior than split_by_regex.h
//
// ======================================================================

#include <algorithm>
#include <functional>
#include <string>

namespace cet {
  template <class OutIter>
  void split(std::string const& s, char c, OutIter dest);

  template <class Pred, class OutIter>
  void split_if(std::string const& s, Pred is_sep, OutIter dest);
}

// ======================================================================

template <class OutIter>
void cet::split(std::string const& s, char c, OutIter dest) {
  split_if(s, [c](char x) { return x == c; }, dest);
}

// ======================================================================

template <class Pred, class OutIter>
void cet::split_if(std::string const& s, Pred is_sep, OutIter dest) {
  auto const b = s.cbegin();
  auto const e = s.cend();

  auto is_not_sep = [is_sep](auto c) { return !is_sep(c); };

  // invariant:  we've found all items in [b..boi)
  // e is an arbitrary value to use as the initializer
  for (auto boi = std::find_if(b, e, is_not_sep), eoi = e; boi != e;
       boi = std::find_if(eoi, e, is_not_sep))  // advance to next non-separator
  {
    // find end of current item:
    eoi = std::find_if(boi, e, is_sep);

    // copy the item formed from characters in [boi..eoi):
    *dest = std::string(boi, eoi);
    ++dest;
  }  // for

}  // split_if<>()

// ======================================================================

#endif /* cetlib_split_h */

// Local variables:
// mode:c++
// End:
