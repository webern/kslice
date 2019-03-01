#pragma once
#include "Extract.h"

namespace kscore {
    void writeFrame( const Frame &frame, std::ostream &os );
    void writeFrames( const Frames &frames, std::ostream &os );
} // namespace kscore