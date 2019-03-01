#pragma once

#include "kscore/IFrameStrategy.h"
#include <string>

namespace kscore {

    // FFProbeStrategy uses an external call to ffprobe to detect the
    // I-Frames in an mpeg file.
    class FFProbeStrategy : public IFrameStrategy {
      public:
        FFProbeStrategy( std::string inputFile );
        // Returns a list of I-Frame indices from an mpeg file. Can throw an
        // exception when an error occurs.
        virtual std::vector<int> getIFrames() override;

      private:
        std::string myInputFile;
    };

} // namespace kscore