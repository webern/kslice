#include <iostream>
#include <vector>
#include <set>

#include "opencv2/opencv.hpp"
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "ezxml/ezxml.h"
#include "boost/algorithm/string.hpp"
#include "Slice.h"


uint64_t hash(const cv::Mat& mat);

int main()
{
    const auto tempDir = boost::filesystem::temp_directory_path();
    const auto tempFilename = boost::lexical_cast<std::string>( boost::uuids::random_generator()() ) + ".xml";
    auto tempPath = boost::lexical_cast<std::string>( tempDir / tempFilename );
    boost::replace_all(tempPath, "\"", "");
    std::cout << tempPath << std::endl;

    std::string cmd = R"(ffprobe -select_streams v -i "../testfiles/1.mp4" -print_format xml -show_entries frame=pict_type,coded_picture_number > )";
    cmd += tempPath;
    auto x = system(cmd.c_str());

    if( x != 0 ) {
        std::cerr << "ffprobe call failed with exit " << x << std::endl;
        return x;
    }

    std::cout << x << std::endl;

    auto xdoc = ezxml::XFactory::makeXDoc();

    try {
        xdoc->loadFile( tempPath );
    } catch (std::exception& e) {
        std::cerr << "parsing of xml doc failed: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "parsing of xml doc failed with unknown exception" << std::endl;
        return 1;
    }

    auto root = xdoc->getRoot();
    if( root->getName() != "ffprobe" )
    {
        return 1;
    }
    const auto framesElementIter = root->begin();
    if( framesElementIter == root->end() )
    {
        return 2;
    }

    auto xmlFrameIter = framesElementIter->begin();
    const auto xmlFrameEnd = framesElementIter->end();
    std::set<int> frameIndices;

    for( ; xmlFrameIter != xmlFrameEnd; ++xmlFrameIter )
    {
        if( xmlFrameIter->getName() != "frame" )
        {
            return 3;
        }

        std::string pictType = "";
        std::string codedPictureNumber = "";

        for( auto a = xmlFrameIter->attributesBegin(); a != xmlFrameIter->attributesEnd(); ++a )
        {
            if( a->getName() == "pict_type" )
            {
                pictType = a->getValue();
            }

            if( a-> getName() == "coded_picture_number" )
            {
                codedPictureNumber = a->getValue();
            }
        }

        if( pictType.empty() || codedPictureNumber.empty() )
        {
            return 4;
        }

        if( pictType == "I" )
        {
            int ix = -1;
            try {
                ix = std::stoi( codedPictureNumber.c_str() );
            } catch (std::exception& e) {
                std::cerr << "encountered a bad number in xml: " << e.what() << std::endl;
                return 5;
            } catch (...) {
                std::cerr << "encountered a bad number in xml with unknown exception type" << std::endl;
                return 6;
            }

            if( ix < 0 ) {
                std::cerr << "negative number encountered" << std::endl;
                return 7;
            }

            frameIndices.insert( ix );
        }

        std::cout << pictType << codedPictureNumber << std::endl;
    }

    if( frameIndices.empty() )
    {
        std::cerr << "no I frames were found" << std::endl;
        return 9;
    }

    std::vector<kslice::Slice> slices;
    cv::VideoCapture cap{ "../testfiles/1.mp4" };


    if( !cap.isOpened() )
    {
        std::cout << "could not open video" << std::endl;
        return 1;
    }

    cv::Mat m;

    for( const auto frameIX : frameIndices )
    {
        const auto ok = cap.set(cv::CAP_PROP_POS_FRAMES, static_cast<double>( frameIX ) );

        if( !ok ) {
            std::cout << "'ok == false' warning, unable to read frame index " << frameIX << std::endl;
            continue;
        }

        const auto isRead = cap.read(m);

        if( !isRead ) {
            std::cout << "'isRead == false' warning, unable to read frame index " << frameIX << std::endl;
            continue;
        }

        const auto seconds = cap.get(cv::CAP_PROP_POS_MSEC) / 1000.0;

        cv::Mat greyMat;
        cv::cvtColor(m, greyMat, cv::COLOR_BGR2GRAY);

        greyMat.resize(1);
        cv::Mat downsampledMap;

        cv::resize(greyMat, downsampledMap, cv::Size{ 32, 32 }, 0, 0, cv::INTER_LINEAR);
        const auto h = hash(greyMat);

        kslice::Slice slice;
        slice.seconds = seconds;
        slice.hash = h;
        slice.data = downsampledMap;
        slices.emplace_back( std::move( slice ) );
    }

    for( const auto& s : slices )
    {
        s.toStream( std::cout );
        std::cout << "\n";
    }
}

