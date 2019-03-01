#include "kscore/Args.h"
#include "kscore/RunProgram.h"
#include <iostream>

int main( int argc, char *argv[] ) {
    kscore::Args args{argc, argv};

    if ( args.isHelp() ) {
        std::cout << args.help() << '\n';
        return 0;
    } else if ( !args.isValid() ) {
        std::cerr << "invalid arguments" << '\n';
        return 1;
    }

    try {
        kscore::RunProgram(args, kscore::makeFFProbe(args.input()), std::cout, std::cerr);
    } catch ( std::exception &e ) {
        std::cerr << e.what() << '\n';
        return 2;
    } catch ( ... ) {
        std::cerr << "unknown exception occurred" << '\n';
        return 3;
    }

    return 0;
}
