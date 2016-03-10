#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <modigliani_core/cylindrical_compartment.h>
#include <modigliani_core/spherical_compartment.h>

TEST_CASE( "Cylindrical compartment", "[cylinder]" ) {
    modigliani_core::Cylindrical_compartment* test = new modigliani_core::Cylindrical_compartment(1,2,4,70,25);
    REQUIRE( test->area() > 6.283185 );
    REQUIRE( test->area() < 6.283186 );
}

TEST_CASE( "Spherical compartment", "[sphere]" ) {
    modigliani_core::Spherical_compartment* test = new modigliani_core::Spherical_compartment(2,70,25);
    REQUIRE( test->area() > 50.2654 );
    REQUIRE( test->area() < 50.2655 );
}
