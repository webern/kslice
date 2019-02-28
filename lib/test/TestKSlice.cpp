#include "File.h"
#include "catch.h"
#include "kscore/kslice.h"
#include <fstream>
#include <sstream>

// This is an end-to-end integration test which requires ffprobe to be
// available.
TEST_CASE( "kslice end-to-end 2x3", "A" ) {
    const auto inputFilepath = kstest::path( "1.mp4" );
    const auto outputFilepath = kstest::tempFilepath();
    const auto inputArg =
        std::string{"--input=\""} + inputFilepath + std::string{"\""};
    const char *argv[] = {"kstest",
                          "--input",
                          inputFilepath.c_str(),
                          "--output",
                          outputFilepath.c_str(),
                          "/Users/mjb/Desktop/testxxx.csv",
                          "--x-size=2",
                          "--y-size=3",
                          nullptr};
    std::stringstream ss;
    const auto exit = kscore::kslice( 8, (char **)argv, ss, std::cerr );
    const auto actual = ss.str();
    CHECK( !actual.empty() );
    CHECK( 0 == exit );

    std::ifstream t( outputFilepath );
    std::string output( ( std::istreambuf_iterator<char>( t ) ),
                        std::istreambuf_iterator<char>() );

    const std::string expected = R"(0.108635,106,95,106,95,106,95
5.97492,66,202,66,202,66,202
)";

    CHECK( expected == output );
}

// This is an end-to-end integration test which requires ffprobe to be
// available.
TEST_CASE( "kslice end-to-end 3x2", "B" ) {
    const auto inputFilepath = kstest::path( "1.mp4" );
    const auto outputFilepath = kstest::tempFilepath();
    const auto inputArg =
        std::string{"--input=\""} + inputFilepath + std::string{"\""};
    const char *argv[] = {"kstest",
                          "--input",
                          inputFilepath.c_str(),
                          "--output",
                          outputFilepath.c_str(),
                          "/Users/mjb/Desktop/testxxx.csv",
                          "--x-size=3",
                          "--y-size=2",
                          nullptr};
    std::stringstream ss;
    const auto exit = kscore::kslice( 8, (char **)argv, ss, std::cerr );
    const auto actual = ss.str();
    CHECK( !actual.empty() );
    CHECK( 0 == exit );

    std::ifstream t( outputFilepath );
    std::string output( ( std::istreambuf_iterator<char>( t ) ),
                        std::istreambuf_iterator<char>() );

    const std::string expected = R"(0.108635,117,56,108,117,56,108
5.97492,162,129,67,162,129,67
)";

    CHECK( expected == output );
}