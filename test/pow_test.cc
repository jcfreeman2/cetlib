#define BOOST_TEST_MODULE ( pow test )
#include "boost/test/auto_unit_test.hpp"

#include "cetlib/pow.h"

using cet::pow;
using cet::square;
using cet::cube;

BOOST_AUTO_TEST_SUITE( pow_test )

BOOST_AUTO_TEST_CASE( zero ) {
  BOOST_CHECK( cet::pow<1>(0   ) == 0    );
  BOOST_CHECK( cet::pow<2>(0u  ) == 0u   );
  BOOST_CHECK( cet::square(0.0 ) == 0.0  );
  BOOST_CHECK( cet::pow<3>(0.0F) == 0.0F );
  BOOST_CHECK( cet::cube  (0.0L) == 0.0L );
  BOOST_CHECK( cet::pow<4>(0UL ) == 0UL  );
  BOOST_CHECK( cet::fourth(0UL ) == 0UL  );
  BOOST_CHECK( cet::pow<5>(0L  ) == 0L   );
  BOOST_CHECK( cet::pow<9>(0.0 ) == 0.0  );
}

BOOST_AUTO_TEST_CASE( one ) {
  BOOST_CHECK( cet::pow<0>(1   ) == 1    );
  BOOST_CHECK( cet::pow<1>(1   ) == 1    );
  BOOST_CHECK( cet::pow<2>(1u  ) == 1u   );
  BOOST_CHECK( cet::square(1.0 ) == 1.0  );
  BOOST_CHECK( cet::pow<3>(1.0F) == 1.0F );
  BOOST_CHECK( cet::cube  (1.0L) == 1.0L );
  BOOST_CHECK( cet::pow<4>(1UL ) == 1UL  );
  BOOST_CHECK( cet::fourth(1UL ) == 1UL  );
  BOOST_CHECK( cet::pow<5>(1L  ) == 1L   );
  BOOST_CHECK( cet::pow<9>(1.0 ) == 1.0  );
}

BOOST_AUTO_TEST_CASE( two ) {
  BOOST_CHECK( cet::pow<0>(2   ) == 1     );
  BOOST_CHECK( cet::pow<1>(2   ) == 2     );
  BOOST_CHECK( cet::pow<2>(2u  ) == 4u    );
  BOOST_CHECK( cet::square(2.0 ) == 4.0   );
  BOOST_CHECK( cet::pow<3>(2.0F) == 8.0F  );
  BOOST_CHECK( cet::cube  (2.0L) == 8.0L  );
  BOOST_CHECK( cet::pow<4>(2UL ) == 16UL  );
  BOOST_CHECK( cet::fourth(2UL ) == 16UL  );
  BOOST_CHECK( cet::pow<5>(2L  ) == 32L   );
  BOOST_CHECK( cet::pow<9>(2.0 ) == 512.0 );
}

BOOST_AUTO_TEST_SUITE_END()
