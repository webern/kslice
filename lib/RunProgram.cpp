#include "kscore/RunProgram.h"
#include "Extract.h"
#include "kscore/Args.h"
#include "kscore/Slice.h"
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

namespace kscore {

    int RunProgram( const Args &args, IFrameStrategyUPtr &&iframeStrategy, std::ostream &stdout,
                    std::ostream &stderr ) {

        if ( args.isHelp() ) {
            return 0;
        }

        if ( !iframeStrategy ) {
            throw std::runtime_error{"iframe strategy is null"};
        }

        const auto frameIndices = iframeStrategy->getIFrames();
        const auto frames = extract( args.input(), frameIndices, Size{args.x(), args.y()} );

        //        std::vector<kslice::Slice> slices;
        //        cv::VideoCapture cap{args.input()};
        //
        //        if ( !cap.isOpened() ) {
        //            stdout << "could not open video" << std::endl;
        //            return 10;
        //        }
        //
        //        cv::Mat m;
        //
        //        for ( const auto frameIX : frameIndices ) {
        //            const auto ok = cap.set( cv::CAP_PROP_POS_FRAMES, static_cast<double>( frameIX ) );
        //
        //            if ( !ok ) {
        //                stdout << "'ok == false' warning, unable to read "
        //                          "frame index "
        //                       << frameIX << std::endl;
        //                continue;
        //            }
        //
        //            const auto isRead = cap.read( m );
        //
        //            if ( !isRead ) {
        //                stdout << "'isRead == false' warning, unable to "
        //                          "read frame index "
        //                       << frameIX << std::endl;
        //                continue;
        //            }
        //
        //            const auto seconds = cap.get( cv::CAP_PROP_POS_MSEC ) / 1000.0;
        //
        //            cv::Mat greyMat;
        //            cv::cvtColor( m, greyMat, cv::COLOR_BGR2GRAY );
        //
        //            greyMat.resize( 1 );
        //            cv::Mat downsampledMap;
        //
        //            cv::resize( greyMat, downsampledMap, cv::Size{args.x(), args.y()}, 0, 0, cv::INTER_LINEAR );
        //
        //            kslice::Slice slice;
        //            slice.seconds = seconds;
        //            slice.data = downsampledMap;
        //            slices.emplace_back( std::move( slice ) );
        //        }

        if ( args.output().empty() ) {
            writeFrames( frames, std::cout );
        } else {
            std::ofstream of;
            of.open( args.output().c_str(), std::ofstream::out );

            if ( !of.is_open() ) {
                stderr << "could not open the output file\n";
                return 11;
            }

            writeFrames( frames, of );
            of.close();
        }

        return 0;
    }
} // namespace kscore