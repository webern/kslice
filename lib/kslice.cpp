#include "kscore/kslice.h"
#include "FFProbeStrategy.h"
#include "Help.h"
#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "ezxml/ezxml.h"
#include "kscore/Args.h"
#include "kscore/Slice.h"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <set>
#include <vector>

namespace kscore {

    int kslice( const Args &args, std::ostream &stdout, std::ostream &stderr ) {

        if ( args.isHelp() ) {
            return 0;
        }

        auto ffprobe = FFProbeStrategy{args.input()};
        const auto frameIndices = ffprobe.getIFrames();

        std::vector<kslice::Slice> slices;
        cv::VideoCapture cap{args.input()};

        if ( !cap.isOpened() ) {
            stdout << "could not open video" << std::endl;
            return 10;
        }

        cv::Mat m;

        for ( const auto frameIX : frameIndices ) {
            const auto ok = cap.set( cv::CAP_PROP_POS_FRAMES,
                                     static_cast<double>( frameIX ) );

            if ( !ok ) {
                stdout << "'ok == false' warning, unable to read "
                          "frame index "
                       << frameIX << std::endl;
                continue;
            }

            const auto isRead = cap.read( m );

            if ( !isRead ) {
                stdout << "'isRead == false' warning, unable to "
                          "read frame index "
                       << frameIX << std::endl;
                continue;
            }

            const auto seconds = cap.get( cv::CAP_PROP_POS_MSEC ) / 1000.0;

            cv::Mat greyMat;
            cv::cvtColor( m, greyMat, cv::COLOR_BGR2GRAY );

            greyMat.resize( 1 );
            cv::Mat downsampledMap;

            cv::resize( greyMat, downsampledMap, cv::Size{args.x(), args.y()},
                        0, 0, cv::INTER_LINEAR );

            kslice::Slice slice;
            slice.seconds = seconds;
            slice.data = downsampledMap;
            slices.emplace_back( std::move( slice ) );
        }

        if ( args.output().empty() ) {
            for ( const auto &s : slices ) {
                s.toStream( std::cout );
                std::cout << "\n";
            }
        } else {
            std::ofstream of;
            of.open( args.output().c_str(), std::ofstream::out );

            if ( !of.is_open() ) {
                stderr << "could not open the output file\n";
                return 11;
            }

            for ( const auto &s : slices ) {
                s.toStream( of );
                of << "\n";
            }
            of.close();
        }

        return 0;
    }
} // namespace kscore