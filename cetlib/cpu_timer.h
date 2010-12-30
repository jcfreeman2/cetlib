#ifndef CETLIB_CPU_TIMER_H
#define CETLIB_CPU_TIMER_H

// ======================================================================
//
// cpu_timer: measure the cpu and wallclock times
//
// ======================================================================

#include <sys/time.h>

namespace cet {
  class cpu_timer;
}

// ======================================================================

class cet::cpu_timer
{
public:
  // --- compiler generates d'tor

  // --- default c'tor
  cpu_timer( );

  // --- accessors:
  bool    is_running( ) const;
  bool    is_stopped( ) const;
  double  elapsed_real_time( ) const;
  double  elapsed_cpu_time ( ) const;
  double  accumulated_real_time( ) const;
  double  accumulated_cpu_time ( ) const;

  // --- mutators:
  void start( );
  void stop( );
  void reset( );

private:
  // --- non-copyable:
  cpu_timer( cpu_timer const & );
  void  operator = ( cpu_timer const & );

  // --- state:
  bool     is_running_;
  timeval  start_real_time_;
  timeval  start_cpu_time_;
  double   accumulated_real_time_;
  double   accumulated_cpu_time_;

};  // cpu_timer

// ======================================================================

inline  bool
  cet::cpu_timer::is_running( ) const
{ return is_running_; }

inline  bool
  cet::cpu_timer::is_stopped( ) const
{ return ! is_running(); }

// ======================================================================

#endif
