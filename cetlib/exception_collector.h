#ifndef CETLIB_EXCEPTION_COLLECTOR_H
#define CETLIB_EXCEPTION_COLLECTOR_H

// ======================================================================
//
// exception_collector: Ensure that each function/functor in a sequence
//                      of calls is invoked even if a previous function
//                      throws.
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
#include <functional>
#include <exception>

// ======================================================================

namespace cet {

  class exception_collector {
  public:
    // compiler generates copy c'tor, copy assignment

    exception_collector( );

#ifdef __GXX_EXPERIMENTAL_CXX0X__
    ~exception_collector( ) noexcept(false);
#else
    ~exception_collector( );
#endif

    // observer:
    bool  has_thrown() const;

    // mutators:
    void  rethrow();
    void  call( std::function<void(void)> );

  private:
    exception  exception_;
    bool       has_thrown_;
  };

}  // namespace cet

// ======================================================================

#endif
