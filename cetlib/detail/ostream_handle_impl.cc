#include "cetlib/detail/ostream_handle_impl.h"

namespace cet::detail {
  // Base
  ostream_handle_base::~ostream_handle_base() = default;

  std::ostream&
  ostream_handle_base::stream()
  {
    return get_stream();
  }

  // Observer
  ostream_observer::ostream_observer(std::ostream& os) : os_{os} {}

  std::ostream&
  ostream_observer::get_stream()
  {
    return os_;
  }
}
