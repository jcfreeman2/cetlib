#include "cetlib/cpu_timer.h"

#include <cstdlib>
#include <unistd.h>  // sleep

//#include "cppunit/extensions/HelperMacros.h"
//#include <cassert>
//#include <iostream>
//#include <string>
//#include <sys/resource.h>

using cet::cpu_timer;


void
  ensure( int which, bool claim )
{
  if( not claim )
    std::exit(which);
}


int
  main( )
{
  {
    cpu_timer  t;
    ensure( 1, t.is_stopped() );
    ensure( 2, ! t.is_running() );
    ensure( 3, t.elapsed_real_time() == 0.0 );
    ensure( 4, t.elapsed_cpu_time() == 0.0 );
    ensure( 5, t.accumulated_real_time() == 0.0 );
    ensure( 6, t.accumulated_cpu_time() == 0.0 );
  }

  {
    cpu_timer  t;
    t.start();
    sleep(2);
    t.stop();
    ensure( 11, t.accumulated_real_time() > 2.0 );
    ensure( 12, t.accumulated_cpu_time()+2.0 <= t.accumulated_real_time() );

    t.start();
    sleep(2);
    ensure( 13, t.accumulated_real_time() > 4.0 );
    t.start(); // redundant
    sleep(1);
    ensure( 14, t.accumulated_real_time() > 5.0 );

    sleep(2);
    t.stop();
    double  real = t.accumulated_real_time();
    double  cpu = t.accumulated_cpu_time();
    t.stop();  // redundant
    ensure( 15, t.accumulated_real_time() == real );
    ensure( 16, t.accumulated_cpu_time()  == cpu );

    t.reset();
    ensure( 17, t.accumulated_real_time() == 0.0 );
    ensure( 18, t.accumulated_cpu_time()  == 0.0 );
  }

  return 0;
}
