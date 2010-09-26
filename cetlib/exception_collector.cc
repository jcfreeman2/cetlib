#include "cetlib/exception_collector.h"
  using cet::exception_collector;


void
  exception_collector::rethrow() const
{
  throw exception_;
}


void
  exception_collector::call( std::function<void(void)> f )
{
  try {
    f();
  }
  catch( cet::exception e ) {
    has_thrown_ = true;
    exception_ << e;
  }
  catch( std::exception e ) {
    has_thrown_ = true;
    exception_ << e.what();
  }
  catch( ... ) {
    has_thrown_ = true;
    exception_ << "Unknown exception";
  }
}  // call()
