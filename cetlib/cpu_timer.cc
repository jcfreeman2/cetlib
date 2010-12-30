// ======================================================================
//
// cpu_timer: measure the cpu and wallclock times
//
// ======================================================================

#include "cetlib/cpu_timer.h"

#include "cetlib/exception.h"
#include <cerrno>
#include <sys/resource.h>

using namespace cet;

// ======================================================================
// helpers:

static  timeval
  real_now( )
{
  timeval now;
  gettimeofday(&now, 0);
  return now;
}

static  timeval
  cpu_now( )
{
  rusage theUsage;
  if( getrusage(RUSAGE_SELF, &theUsage) != 0 ) {
    throw exception("cpu_timer", "Failure in get_current_stats") << errno;
  }

  timeval now;
  now.tv_sec  = theUsage.ru_stime.tv_sec  + theUsage.ru_utime.tv_sec;
  now.tv_usec = theUsage.ru_stime.tv_usec + theUsage.ru_utime.tv_usec;

  return now;
}

static  double
  operator - ( timeval const & t1, timeval const & t2 )
{
  static  double const  microsec_per_sec = 1E-6;
  return t1.tv_sec - t2.tv_sec
       + (t1.tv_usec - t2.tv_usec) * microsec_per_sec;
}

// ======================================================================
// c'tor:

cpu_timer::cpu_timer()
: is_running_           ( false )
, start_real_time_      ( )
, start_cpu_time_       ( )
, accumulated_real_time_( 0.0 )
, accumulated_cpu_time_ ( 0.0 )
{
  start_real_time_.tv_sec = 0, start_real_time_.tv_usec = 0;
  start_cpu_time_ .tv_sec = 0, start_cpu_time_ .tv_usec = 0;
}

// ======================================================================
// accessors:

double
  cpu_timer::elapsed_real_time( ) const
{ return real_now() - start_real_time_; }

double
  cpu_timer::elapsed_cpu_time( ) const
{ return cpu_now() - start_cpu_time_; }

double
  cpu_timer::accumulated_real_time( ) const
{
  return is_stopped()  ?  accumulated_real_time_
                       :  accumulated_real_time_ + elapsed_real_time();
}

double
  cpu_timer::accumulated_cpu_time( ) const
{
  return is_stopped()  ?  accumulated_cpu_time_
                       :  accumulated_cpu_time_ + elapsed_cpu_time();
}

// ======================================================================
// mutators:

void
  cpu_timer::start( )
{
  if( is_stopped() ) start_real_time_ = real_now()
                   , start_cpu_time_  = cpu_now()
                   , is_running_ = true
                   ;
}

void
  cpu_timer::stop( )
{
  if( is_running() ) accumulated_real_time_ += elapsed_real_time()
                   , accumulated_cpu_time_  += elapsed_cpu_time()
                   , is_running_ = false
                   ;
}

void
  cpu_timer::reset( )
{
  accumulated_real_time_ = 0.0;
  accumulated_cpu_time_  = 0.0;
}

// ======================================================================
