#include "catch.h"
#include "kscore/Args.h"
#include "kscore/kslice.h"
#include <sstream>

TEST_CASE( "kslice help produces output", "kslice help" ) {
    const char *argv[] = {"kstest", "--help"};
    kscore::Args args{2, (char **)argv};
    std::stringstream ss;
    const auto exit = kscore::kslice( args, ss, std::cerr );
    const auto actual = args.isHelp() ? args.help() : "";
    CHECK( !actual.empty() );
    CHECK( 0 == exit );
}