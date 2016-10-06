#define CATCH_CONFIG_MAIN 
#include "test/catch.hh"
#include <assert.h>

TEST_CASE( "Sanity", "[test]" ) {
    REQUIRE(true == true);
}
