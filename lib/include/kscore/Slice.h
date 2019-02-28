#pragma once

#include <opencv2/core/mat.hpp>

#include <cstdint>
#include <iostream>
#include <set>
#include <vector>

namespace kslice {

    struct Slice {
      public:
        uint64_t hash;
        double seconds;
        cv::Mat data;

        void toStream( std::ostream &os ) const;
    };

} // namespace kslice