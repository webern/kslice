#pragma once
#include <cstdint>
#include <memory>
#include <vector>

namespace kscore {
    class IFrameStrategy;
    using IFrameStrategyUPtr = std::unique_ptr<IFrameStrategy>;

    // returns a strategy which uses an external call to FFProbe
    IFrameStrategyUPtr makeFFProbe( std::string inputFile );

    // IFrameStrategy produces a list of I-Frame indices in an mpeg
    // file.
    class IFrameStrategy {
      public:
        virtual ~IFrameStrategy() = default;
        // Returns a list of I-Frame indices from an mpeg file. Can throw an
        // exception when an error occurs.
        virtual std::vector<int> getIFrames() = 0;
    };

} // namespace kscore