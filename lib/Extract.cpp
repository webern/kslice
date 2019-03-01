#include "Extract.h"
#include "kscore/Slice.h"
#include "opencv2/opencv.hpp"

namespace kscore {
    Frames extract( const std::string &file, const std::vector<int> &frameIndices, const Size &size ) {

        cv::VideoCapture cap{file};

        if ( !cap.isOpened() ) {
            throw std::runtime_error{"could not open video"};
        }

        cv::Mat m;

        Frames frames;
        frames.reserve( frameIndices.size() );

        for ( const auto frameIX : frameIndices ) {
            const auto ok = cap.set( cv::CAP_PROP_POS_FRAMES, static_cast<double>( frameIX ) );

            if ( !ok ) {
                // should we throw?
                continue;
            }

            const auto isRead = cap.read( m );

            if ( !isRead ) {
                // should we throw?
                continue;
            }

            const auto seconds = cap.get( cv::CAP_PROP_POS_MSEC ) / 1000.0;

            cv::Mat greyMat;
            cv::cvtColor( m, greyMat, cv::COLOR_BGR2GRAY );

            greyMat.resize( 1 );
            cv::Mat downsampledMap;

            cv::resize( greyMat, downsampledMap, cv::Size{size.xPixels, size.yPixels}, 0, 0, cv::INTER_LINEAR );

            kslice::Slice slice;
            slice.seconds = seconds;
            slice.data = downsampledMap;

            PixelData pixelData;
            pixelData.reserve( static_cast<size_t>( downsampledMap.rows ) );

            for ( int i = 0; i < downsampledMap.rows; i++ ) {
                std::vector<uint8_t> row;
                row.reserve( static_cast<size_t>( downsampledMap.cols ) );
                for ( int j = 0; j < downsampledMap.cols; j++ ) {
                    row.push_back( downsampledMap.at<uchar>( i, j ) );
                }
                pixelData.emplace_back( std::move( row ) );
            }

            Frame frame = std::make_pair( seconds, std::move( pixelData ) );
            frames.emplace_back( std::move( frame ) );
        }
        return frames;
    }
} // namespace kscore