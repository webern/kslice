#include <iostream>
#include <vector>
#include <set>

#include "opencv2/opencv.hpp"
#include <boost/program_options.hpp>
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include "ezxml/ezxml.h"
#include "boost/algorithm/string.hpp"
#include "Slice.h"
#include "Help.h"

int main(int argc, char *argv[])
{
    boost::program_options::options_description desc{"Options"};
    desc.add_options()
            ("help", "Help screen")
            ("input", boost::program_options::value<std::string>(), "Input file")
            ("output", boost::program_options::value<std::string>(), "Output file")
            ("x-size", boost::program_options::value<int>(), "Grid size x")
            ("y-size", boost::program_options::value<int>(), "Grid size y");

    boost::program_options::variables_map vm;
    boost::program_options::store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if( vm.count( "help" ) > 0 )
    {
        std::cout << kslice::help << '\n';
        return 0;
    }

    std::string input = "";
    std::string output = "";
    int xSize = 0;
    int ySize = 0;

    if( vm.count( "input" ) == 0 )
    {
        std::cout << "input is required\n";
        std::cout << kslice::help << '\n';
        return 1;
    }
    else
    {
        input = vm["input"].as<std::string>();
    }

    if( vm.count( "output" ) == 1 )
    {
        output = vm["output"].as<std::string>();
    }

    if( vm.count( "x-size" ) == 1 )
    {
        xSize = vm["x-size"].as<int>();
    }

    if( vm.count( "y-size" ) == 1 )
    {
        ySize = vm["y-size"].as<int>();
    }

    if( xSize <= 0 )
    {
        xSize = 32;
    }

    if( ySize <= 0 )
    {
        ySize = 32;
    }


    const auto tempDir = boost::filesystem::temp_directory_path();
    const auto tempFilename = boost::lexical_cast<std::string>( boost::uuids::random_generator()() ) + ".xml";
    auto tempPath = boost::lexical_cast<std::string>( tempDir / tempFilename );
    boost::replace_all(tempPath, "\"", "");

    std::string cmd = R"(ffprobe -select_streams v -i "####INPUTFILE####" -print_format xml -show_entries frame=pict_type,coded_picture_number > )";
    boost::replace_all(cmd, "####INPUTFILE####", input);
    cmd += "\"" + tempPath + "\"";
    std::cout << cmd << std::endl;
    auto x = system(cmd.c_str());

    if( x != 0 ) {
        std::cerr << "ffprobe call failed with exit " << x << std::endl;
        return x;
    }

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

        std::string pictType;
        std::string codedPictureNumber;

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
                ix = std::stoi( codedPictureNumber );
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
        return 10;
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

        cv::resize(greyMat, downsampledMap, cv::Size{ xSize, ySize }, 0, 0, cv::INTER_LINEAR);

        kslice::Slice slice;
        slice.seconds = seconds;
        slice.data = downsampledMap;
        slices.emplace_back( std::move( slice ) );
    }

    std::ostream* os = nullptr;

    if( !output.empty() )
    {
        std::ofstream of{ output };

        if( !of.is_open() )
        {
            std::cerr << "could not open the output file\n";
            return 11;
        }

        os = &of;
    }

    if( os == nullptr )
    {
        os = &std::cout;
    }

    for( const auto& s : slices )
    {
        s.toStream( *os );
        ( *os ) << "\n";
    }
}

