#ifndef CETLIB_MAKE_UNIQUE_H
#define CETLIB_MAKE_UNIQUE_H

namespace cet
{
  // Return a std::unique_ptr<T>. This should be essentially identical
  // to what will be provided in C++14.

  template <class T, class ... Args>
  std::unique_ptr<T>
  make_unique(Args&& ... args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }
}

#endif
