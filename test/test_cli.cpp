#define BOOST_TEST_MODULE cli_test
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(always_true)
{
    BOOST_CHECK(2*2==4);
}