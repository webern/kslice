#include "Write.h"
#include <ostream>

namespace kscore {
    void writeFrame( const Frame &frame, std::ostream &os ) {
        os << frame.first << ',';
        bool isFirst = true;
        for ( const auto &row : frame.second ) {
            for ( const auto &val : row ) {
                if ( !isFirst ) {
                    os << ',';
                }
                os << static_cast<int>( val );
                isFirst = false;
            }
        }
    }
    void writeFrames( const Frames &frames, std::ostream &os ) {
        for ( const auto &frame : frames ) {
            writeFrame( frame, os );
            os << '\n';
        }
    }
} // namespace kscore
