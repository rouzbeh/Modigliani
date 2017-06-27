#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <modigliani/cylindrical_compartment.h>
#include <modigliani/spherical_compartment.h>

TEST_CASE( "Cylindrical compartment", "[cylinder]" ) {
    modigliani::Cylindrical_compartment* test = new modigliani::Cylindrical_compartment(1,2,4,70,25);
    REQUIRE( test->area() > 6.283185 );
    REQUIRE( test->area() < 6.283186 );
}

TEST_CASE( "Spherical compartment", "[sphere]" ) {
    modigliani::Spherical_compartment* test = new modigliani::Spherical_compartment(2,70,25);
    REQUIRE( test->area() > 50.2654 );
    REQUIRE( test->area() < 50.2655 );
}
