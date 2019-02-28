#include "catch.h"
#include "kscore/kslice.h"
#include <sstream>

TEST_CASE( "kslice help produces output", "kslice help" ) {
    const char *argv[] = {"kstest", "--help"};
    std::stringstream ss;
    const auto exit = kscore::kslice( 2, (char **)argv, ss, std::cerr );
    const auto actual = ss.str();
    CHECK( !actual.empty() );
    CHECK( 0 == exit );
}