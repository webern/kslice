#include "FFProbeStrategy.h"
#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/program_options.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "ezxml/ezxml.h"

namespace kscore {

    IFrameStrategyUPtr makeFFProbe( std::string inputFile ) {
        return std::make_unique<FFProbeStrategy>( std::move( inputFile ) );
    }

    FFProbeStrategy::FFProbeStrategy( std::string inputFile )
        : myInputFile{std::move( inputFile )} {}

    std::vector<int> FFProbeStrategy::getIFrames() {

        const auto tempDir = boost::filesystem::temp_directory_path();
        const auto tempFilename = boost::lexical_cast<std::string>(
                                      boost::uuids::random_generator()() ) +
                                  ".xml";
        auto tempPath =
            boost::lexical_cast<std::string>( tempDir / tempFilename );
        boost::replace_all( tempPath, "\"", "" );

        std::string cmd =
            R"(ffprobe -select_streams v -i "####INPUTFILE####" -loglevel "-8" -print_format xml -show_entries frame=pict_type,coded_picture_number > )";
        boost::replace_all( cmd, "####INPUTFILE####", myInputFile );
        cmd += "\"" + tempPath + "\"";
        auto sysExit = system( cmd.c_str() );

        if ( sysExit != 0 ) {
            std::stringstream msg;
            msg << "ffprobe call failed with exit code " << sysExit << '\n';
            throw std::runtime_error{msg.str()};
        }

        auto xdoc = ezxml::XFactory::makeXDoc();
        xdoc->loadFile( tempPath );
        auto root = xdoc->getRoot();
        if ( root->getName() != "ffprobe" ) {
            throw std::runtime_error{
                "unexpected xml format 'ffprobe' not found"};
        }
        const auto framesElementIter = root->begin();
        if ( framesElementIter == root->end() ) {
            throw std::runtime_error{"unexpected xml format no frames found"};
        }

        auto xmlFrameIter = framesElementIter->begin();
        const auto xmlFrameEnd = framesElementIter->end();
        std::vector<int> frameIndices;

        for ( ; xmlFrameIter != xmlFrameEnd; ++xmlFrameIter ) {
            if ( xmlFrameIter->getName() != "frame" ) {
                throw std::runtime_error{
                    "unexpected xml format 'frame' not found"};
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
                throw std::runtime_error{
                    "unexpected xml format bad values encountered"};
            }

            if ( pictType == "I" ) {
                int ix = -1;
                try {
                    ix = std::stoi( codedPictureNumber );
                } catch ( std::exception &e ) {
                    std::stringstream msg;
                    msg << "encountered a bad number in xml: " << e.what()
                        << std::endl;
                    throw std::runtime_error{msg.str()};
                } catch ( ... ) {
                    std::stringstream msg;
                    msg << "encountered a bad number in xml "
                           "with unknown exception type"
                        << std::endl;
                    throw std::runtime_error{msg.str()};
                }

                if ( ix < 0 ) {
                    throw std::runtime_error{
                        "unexpected xml format negative number encountered"};
                }

                frameIndices.push_back( ix );
            }
        }

        if ( frameIndices.empty() ) {
            throw std::runtime_error{"no I frames were found"};
        }
        return frameIndices;
    }
} // namespace kscore