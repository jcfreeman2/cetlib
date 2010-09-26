#ifndef CETLIB__EXCEPTION_COLLECTOR_H
#define CETLIB__EXCEPTION_COLLECTOR_H

// ======================================================================
//
// exception_collector:  ensure that each function/functor in a sequence
//                       of calls is invoked even if a previous function
//                       throws.
//
// Each function/functor must take no arguments and return a void.  The
// exception strings are saved in a cms::Exception for optional rethrow.
// Example:
//
//   exception_collector ec;
//
//   ec.call( bind(&MyClass::myFunction, myClassPtr) );
//   ec.call( bind(&MyClass::myOtherFunction, myClassPtr, myArgPtr) );
//   ec.call( bind(&myFreeFunction, myArgPtr) );
//   if( ec.has_thrown() ) ec.rethrow();
//
// All three functions will be called before any exception escapes.
//
// ======================================================================


#include "cetlib/exception.h"
#include "cpp0x/functional.h"
#include <exception>


// ======================================================================


namespace cet {

  class exception_collector {
  public:
      exception_collector()
    : exception_( std::string() )
    , has_thrown_( false )
    { }
    ~exception_collector() {}

    bool  has_thrown() const { return has_thrown_; }
    void  rethrow() const;
    void  call( std::function<void(void)> );

  private:
    exception  exception_;
    bool       has_thrown_;
  };

}  // namespace cet


// ======================================================================


#endif  // CETLIB__EXCEPTION_COLLECTOR_H
