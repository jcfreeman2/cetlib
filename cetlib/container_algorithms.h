#ifndef CETLIB_CONTAINER_ALGORITHMS_H
#define CETLIB_CONTAINER_ALGORITHMS_H

// ======================================================================
//
// container_algorithms: Function templates wrapping standard algorithms
//
// Avoids incompatible iterators and reduces some duplication
//
// ======================================================================

#include <algorithm>

// ----------------------------------------------------------------------
// wrapper for std::for_each
namespace cet {
  template< class FwdCont, class Func >
  inline Func
    for_all( FwdCont & s, Func f )
  { return std::for_each(s.begin(), s.end(), f); }
}

// ----------------------------------------------------------------------
// wrappers for std::copy
namespace cet {
  template< class FwdCont, class FwdIter >
  inline FwdIter
    copy_all( FwdCont & s, FwdIter it )
  { return std::copy(s.begin(), s.end(), it); }
}

// ----------------------------------------------------------------------
// wrappers for std::find
namespace cet {
  template< class FwdCont, class Datum >
  inline typename FwdCont::const_iterator
    find_in_all( FwdCont const & s, Datum const & d )
  { return std::find(s.begin(), s.end(), d); }

  template< class FwdCont, class Datum >
  inline typename FwdCont::iterator
    find_in_all( FwdCont & s, Datum const & d )
  { return std::find(s.begin(), s.end(), d); }

  template< class FwdCont, class Datum >
  inline bool
    search_all( FwdCont const & s, Datum const & d )
  { return std::find(s.begin(), s.end(), d) != s.end(); }
}

// ----------------------------------------------------------------------
// wrapper for std::binary_search
namespace cet {
  template< class FwdCont, class Datum >
  inline bool
    binary_search_all( FwdCont const & s, Datum const & d )
  { return std::binary_search(s.begin(), s.end(), d); }
}

// ----------------------------------------------------------------------
// wrappers for std::lower_bound
namespace cet {
  template< class FwdCont, class Datum >
  inline typename FwdCont::const_iterator
    lower_bound_all( FwdCont const & s, Datum const & d )
  { return std::lower_bound(s.begin(), s.end(), d); }

  template< class FwdCont, class Datum >
  inline typename FwdCont::iterator
    lower_bound_all( FwdCont & s, Datum const & d )
  { return std::lower_bound(s.begin(), s.end(), d); }

  template< class FwdCont, class Datum, class Pred >
  inline typename FwdCont::const_iterator
    lower_bound_all( FwdCont const & s, Datum const & d, Pred p )
  { return std::lower_bound(s.begin(), s.end(), d, p); }

  template< class FwdCont, class Datum, class Pred >
  inline typename FwdCont::iterator
    lower_bound_all( FwdCont & s, Datum const & d, Pred p )
  { return std::lower_bound(s.begin(), s.end(), d, p); }
}

// ----------------------------------------------------------------------
// wrappers for std::sort
namespace cet {
  template< class RandCont >
  inline void
    sort_all( RandCont & s )
  { std::sort(s.begin(), s.end()); }

  template< class RandCont, class Pred >
  inline void
    sort_all( RandCont & s, Pred p )
  { std::sort(s.begin(), s.end(), p); }
}

// ----------------------------------------------------------------------
// wrappers for std::stable_sort
namespace cet {
  template< class RandCont >
  inline void
    stable_sort_all( RandCont & s )
  { std::stable_sort(s.begin(), s.end()); }

  template< class RandCont, class Pred >
  inline void
    stable_sort_all( RandCont & s, Pred p )
  { std::stable_sort(s.begin(), s.end(), p); }
}

// ======================================================================

#endif
