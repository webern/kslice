#include "kscore/kslice.h"
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

    int kslice( int argc, char *argv[], std::ostream &stdout,
                std::ostream &stderr ) {
        Args args{argc, argv};

        if ( args.isHelp() ) {
            stdout << args.help() << '\n';
            return 0;
        }

        if ( !args.isValid() ) {
            stderr << "arguments are not valid: " << args.error() << '\n';
            return 1;
        }

        const auto tempDir = boost::filesystem::temp_directory_path();
        const auto tempFilename = boost::lexical_cast<std::string>(
                                      boost::uuids::random_generator()() ) +
                                  ".xml";
        auto tempPath =
            boost::lexical_cast<std::string>( tempDir / tempFilename );
        boost::replace_all( tempPath, "\"", "" );

        std::string cmd =
            R"(ffprobe -select_streams v -i "####INPUTFILE####" -loglevel "-8" -print_format xml -show_entries frame=pict_type,coded_picture_number > )";
        boost::replace_all( cmd, "####INPUTFILE####", args.input() );
        cmd += "\"" + tempPath + "\"";
        stdout << cmd << std::endl;
        auto x = system( cmd.c_str() );

        if ( x != 0 ) {
            stderr << "ffprobe call failed with exit " << x << std::endl;
            return x;
        }

        auto xdoc = ezxml::XFactory::makeXDoc();

        try {
            xdoc->loadFile( tempPath );
        } catch ( std::exception &e ) {
            stderr << "parsing of xml doc failed: " << e.what() << std::endl;
            return 1;
        } catch ( ... ) {
            stderr << "parsing of xml doc failed with unknown exception"
                   << std::endl;
            return 1;
        }

        auto root = xdoc->getRoot();
        if ( root->getName() != "ffprobe" ) {
            return 1;
        }
        const auto framesElementIter = root->begin();
        if ( framesElementIter == root->end() ) {
            return 2;
        }

        auto xmlFrameIter = framesElementIter->begin();
        const auto xmlFrameEnd = framesElementIter->end();
        std::set<int> frameIndices;

        for ( ; xmlFrameIter != xmlFrameEnd; ++xmlFrameIter ) {
            if ( xmlFrameIter->getName() != "frame" ) {
                return 3;
            }

            std::string pictType;
            std::string codedPictureNumber;

            for ( auto a = xmlFrameIter->attributesBegin();
                  a != xmlFrameIter->attributesEnd(); ++a ) {
                if ( a->getName() == "pict_type" ) {
                    pictType = a->getValue();
                }

                if ( a->getName() == "coded_picture_number" ) {
                    codedPictureNumber = a->getValue();
                }
            }

            if ( pictType.empty() || codedPictureNumber.empty() ) {
                return 4;
            }

            if ( pictType == "I" ) {
                int ix = -1;
                try {
                    ix = std::stoi( codedPictureNumber );
                } catch ( std::exception &e ) {
                    stderr << "encountered a bad number in xml: " << e.what()
                           << std::endl;
                    return 5;
                } catch ( ... ) {
                    stderr << "encountered a bad number in xml "
                              "with unknown exception type"
                           << std::endl;
                    return 6;
                }

                if ( ix < 0 ) {
                    stderr << "negative number encountered" << std::endl;
                    return 7;
                }

                frameIndices.insert( ix );
            }
        }

        if ( frameIndices.empty() ) {
            stderr << "no I frames were found" << std::endl;
            return 9;
        }

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