#include "kscore/Args.h"
#include "Help.h"
#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"

namespace kscore {
    Args::Args( int argc, char *argv[] )
        : myIsHelp{false}, myInput{}, myOutput{}, myX{-1}, myY{-1} {

        boost::program_options::options_description desc{"Options"};
        desc.add_options()( "help", "Help screen" )(
            "input", boost::program_options::value<std::string>(),
            "Input file" )( "output",
                            boost::program_options::value<std::string>(),
                            "Output file" )(
            "x-size", boost::program_options::value<int>(), "Grid size x" )(
            "y-size", boost::program_options::value<int>(), "Grid size y" );

        boost::program_options::variables_map vm;
        boost::program_options::store( parse_command_line( argc, argv, desc ),
                                       vm );
        notify( vm );

        myIsHelp = vm.count( "help" ) > 0;

        if ( myIsHelp ) {
            return;
        }

        if ( vm.count( "input" ) == 0 ) {
            myError = "input is required";
            return;
        } else {
            myInput = vm["input"].as<std::string>();
        }

        if ( vm.count( "output" ) == 1 ) {
            myOutput = vm["output"].as<std::string>();
        }

        if ( vm.count( "x-size" ) == 1 ) {
            myX = vm["x-size"].as<int>();
        }

        if ( vm.count( "y-size" ) == 1 ) {
            myY = vm["y-size"].as<int>();
        }

        if ( myX <= 0 ) {
            myX = 32;
        }

        if ( myY <= 0 ) {
            myY = 32;
        }
    }

    Args::Args( bool isHelp, std::string input, std::string output, int x,
                int y )
        : myIsHelp{isHelp}, myInput{input}, myOutput{output}, myX{x}, myY{y} {
        if ( myX <= 0 ) {
            myX = 32;
        }
        if ( myY <= 0 ) {
            myY = 32;
        }
    }

    bool Args::isValid() const {
        if ( myX < 0 ) {
            return false;
        }

        if ( myY < 0 ) {
            return false;
        }

        if ( myInput.empty() ) {
            return false;
        }

        return myError.empty();
    }

    std::string Args::error() const { return myError; }
    std::string Args::help() const { return kslice::help; }
    bool Args::isHelp() const { return myIsHelp; }
    const std::string &Args::input() const { return myInput; }
    const std::string &Args::output() const { return myOutput; }
    int Args::x() const { return myX; }
    int Args::y() const { return myY; }

} // namespace kscore