#include "kscore/Args.h"
#include "kscore/kslice.h"
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

    return kscore::kslice( args, std::cout, std::cerr );
}
