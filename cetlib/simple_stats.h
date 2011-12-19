#ifndef CETLIB_SIMPLE_STATS_H
#define CETLIB_SIMPLE_STATS_H

// ======================================================================
//
// simple_stats: yield simple statistics from individually-presented data
//
// ======================================================================

#include <cstddef>  // size_t

namespace cet {

  class simple_stats;

}

// ======================================================================

class cet::simple_stats
{
public:
  // c'tors:
  simple_stats( ) noexcept  { reset(); }
  simple_stats( double x ) noexcept  { reset(); use(x); }

  // use compiler-generated copy/move/d'tor

  // statistics observers:
  std::size_t  size( )  const noexcept  { return n_; }
  double       min( )   const noexcept  { return min_; }
  double       max( )   const noexcept  { return max_; }
  double       small( ) const noexcept  { return small_; }
  double       sum( )   const noexcept  { return sum_; }
  double       sumsq( ) const noexcept  { return sumsq_; }

  // statistics calculators:
  double       mean( )  const noexcept;
  double       rms( )   const noexcept;

  // mutators:
  void  reset( ) noexcept;
  void  use( double ) noexcept;

private:
  std::size_t  n_;             // counter
  double  min_, max_, small_;  // extrema
  double  sum_, sumsq_;        // accumulations

  // helpers:
  double  sqr( double x )  { return x * x; }

};  // simple_stats

// ======================================================================

#endif  // CETLIB_SIMPLE_STATS_H
