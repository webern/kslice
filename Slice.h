#pragma once

#include <cstdint>
#include <opencv2/core/mat.hpp>

namespace kslice {

    struct Slice {
    public:
        uint64_t hash;
        double seconds;
        cv::Mat data;
    };

    // takes N equally-spaced samples from the slice set provided that slices.size() >= N
    inline std::vector<Slice> sample(const std::set<Slice>& slices, int inNumSamples) {
        if( inNumSamples <= 0 ) {
            return std::vector<Slice>{};
        }

        std::vector<Slice> result;
        const auto div = slices.size() / static_cast<size_t>( inNumSamples );
        const auto stride = div > 0 ? div : 1;
        size_t current = 0;

        for ( const auto& slice : slices )
        {
            if( current % stride == 0 )
            {
                result.emplace_back( slice );
            }
            current++;
        }

        const auto less = [&]( Slice& a, Slice& b ) {
            return a.seconds < b.seconds;
        };

        std::sort(std::begin(result), std::end(result), less);
        return result;
    }


    inline bool operator<(const Slice &a, const Slice &b) {
        if (a.hash < b.hash) {
            return true;
        }

        if (a.hash > b.hash) {
            return false;
        }

        return a.seconds < b.seconds;
    }

    inline bool operator>(const Slice &a, const Slice &b) {
        return b < a;
    }

    inline bool operator==(const Slice &a, const Slice &b) {
        return !(a < b) && !(b < a);
    }

    inline bool operator!=(const Slice &a, const Slice &b) {
        return !(a == b);
    }

    inline bool operator<=(const Slice &a, const Slice &b) {
        return (a < b) || (a == b);
    }

    inline bool operator>=(const Slice &a, const Slice &b) {
        return (a > b) || (a == b);
    }

}