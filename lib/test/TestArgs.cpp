#include "catch.h"
#include "kscore/Args.h"
#include <sstream>

TEST_CASE( "Args help", "" ) {
    const char *argv[] = {"kstest", "--help"};
    kscore::Args args{2, (char **)argv};
    CHECK( args.isHelp() );
    CHECK( !args.isValid() );
}

TEST_CASE( "Args valid", "" ) {
    const char *argv[] = {
        "kstest",          "--input",    "INPUT_WHATEVER", "--output",
        "OUTPUT_WHATEVER", "--x-size=3", "--y-size=2",     nullptr};
    kscore::Args args{7, (char **)argv};
    CHECK( !args.isHelp() );
    CHECK( args.isValid() );
    CHECK( "INPUT_WHATEVER" == args.input() );
    CHECK( "OUTPUT_WHATEVER" == args.output() );
    CHECK( 3 == args.x() );
    CHECK( 2 == args.y() );
}

TEST_CASE( "Args valid default xy", "" ) {
    const char *argv[] = {"kstest",   "--input",         "INPUT_WHATEVER",
                          "--output", "OUTPUT_WHATEVER", "--x-size=-1"};
    kscore::Args args{6, (char **)argv};
    CHECK( !args.isHelp() );
    CHECK( args.isValid() );
    CHECK( "INPUT_WHATEVER" == args.input() );
    CHECK( "OUTPUT_WHATEVER" == args.output() );
    CHECK( 32 == args.x() );
    CHECK( 32 == args.y() );
}

TEST_CASE( "Args valid - no output", "" ) {
    const char *argv[] = {"kstest", "--input", "INPUT_WHATEVER", "--x-size=31",
                          "--y-size=34"};
    kscore::Args args{5, (char **)argv};
    CHECK( !args.isHelp() );
    CHECK( args.isValid() );
    CHECK( "INPUT_WHATEVER" == args.input() );
    CHECK( args.output().empty() );
    CHECK( 31 == args.x() );
    CHECK( 34 == args.y() );
}

TEST_CASE( "Args alternate constructor", "" ) {
    kscore::Args args{false, "INPUT_WHATEVER", "OUTPUT_WHATEVER", 13, -1};
    CHECK( !args.isHelp() );
    CHECK( args.isValid() );
    CHECK( "INPUT_WHATEVER" == args.input() );
    CHECK( "OUTPUT_WHATEVER" == args.output() );
    CHECK( 13 == args.x() );
    CHECK( 32 == args.y() );
}