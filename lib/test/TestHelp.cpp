#include "catch.h"
#include "kscore/Args.h"
#include "kscore/IFrameStrategy.h"
#include "kscore/RunProgram.h"
#include <sstream>

TEST_CASE( "kslice help produces output", "kslice help" ) {
    const char *argv[] = {"kstest", "--help"};
    kscore::Args args{2, (char **)argv};
    std::stringstream ss;
    const auto exit = kscore::RunProgram(args, kscore::makeFFProbe(args.input()), ss, std::cerr);
    const auto actual = args.isHelp() ? args.help() : "";
    CHECK( !actual.empty() );
    CHECK( 0 == exit );
}