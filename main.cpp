#include "kscore/kslice.h"
#include <iostream>

int main( int argc, char *argv[] ) {
    return kscore::kslice( argc, argv, std::cout, std::cerr );
}
