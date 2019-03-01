#pragma once
#include <cstdint>
#include <vector>

namespace kscore {
    using TimestampSeconds = double;
    using PixelData = std::vector<std::vector<uint8_t>>;
    using Frame = std::pair<TimestampSeconds, PixelData>;
    using Frames = std::vector<Frame>;

    struct Size {
        int xPixels;
        int yPixels;
    };

    // converts the input video to grayscale, downsamples it to 'size'. only processes those frames lised in
    // 'frameIndices'. extract throws exceptions.
    Frames extract( const std::string &file, const std::vector<int> &frameIndices, const Size &size );
} // namespace kscore