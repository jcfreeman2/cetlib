// ======================================================================
//
// simple_stats: yield simple statistics from individually-presented data
//
// ======================================================================

#include "cetlib/simple_stats.h"

#include <algorithm>
#include <cmath>
#include <limits>

using cet::simple_stats;

// ----------------------------------------------------------------------
// statistics calculators:

double  simple_stats::mean( ) const noexcept
{
  return sum_ / double(n_);
}

double  simple_stats::rms( ) const noexcept
{
  return std::sqrt( sumsq_ / double(n_) );
}


// ----------------------------------------------------------------------
// mutators:

void  simple_stats::reset( ) noexcept
{
  n_     = 0u;
  min_   = + std::numeric_limits<double>::infinity();
  max_   = - std::numeric_limits<double>::infinity();
  small_ = + std::numeric_limits<double>::infinity();
  sum_   = 0.0;
  sumsq_ = 0.0;
}

void  simple_stats::use( double x ) noexcept
{
  ++n_;
  min_   = std::min(x, min_);
  max_   = std::max(x, max_);
  small_ = std::min( std::abs(x), small_);
  sum_   += x;
  sumsq_ += x * x;
}
