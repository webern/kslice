#include "include/kscore/Slice.h"

namespace kslice {

void Slice::toStream( std::ostream &os ) const {
    os << seconds << ",";

    for ( int r = 0; r < data.rows; r++ ) {
        // Loop over all columns
        for ( int c = 0; c < data.cols; c++ ) {
            const auto pixel = data.at<uchar>( r, c );
            if ( r > 0 || c > 0 ) {
                os << ",";
            }

            os << (int)pixel;
        }
    }
}

} // namespace kslice