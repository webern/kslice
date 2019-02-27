#include <iostream>
#include <vector>
#include <set>

#include "opencv2/opencv.hpp"
#include "boost/filesystem.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"

#include "Slice.h"


uint64_t hash(const cv::Mat& mat);

int main()
{
    const auto tempDir = boost::filesystem::temp_directory_path();
    const auto tempFilename = boost::lexical_cast<std::string>( boost::uuids::random_generator()() ) + ".json";
    const auto tempPath = tempDir / tempFilename;
    std::cout << tempPath << std::endl;

    std::string cmd = R"(ffprobe -select_streams v -i "../testfiles/1.mp4" -print_format xml -show_entries frame=pict_type,coded_picture_number > )";
    cmd += boost::lexical_cast<std::string>( tempPath );
    auto x = system(cmd.c_str());

    if( x != 0 ) {
        std::cerr << "ffprobe call failed with exit " << x << std::endl;
        return x;
    }

    std::cout << x << std::endl;
    std::set<kslice::Slice> slices;
    cv::VideoCapture cap{ "../testfiles/1.mp4" };


    if( !cap.isOpened() )
    {
        std::cout << "could not open video" << std::endl;
        return 1;
    }

    cv::Mat m;

    while ( true )
    {
        const auto isRead = cap.read(m);
        const auto seconds = cap.get(cv::CAP_PROP_POS_MSEC) / 1000.0;

        if( !isRead )
        {
            break;
        }

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
        slices.emplace( std::move( slice ) );
//        std::cout << h << std::endl;
    }

    const auto samples = sample(slices, 4);

    for( const auto& s : samples )
    {
        s.toStream( std::cout );
        std::cout << "\n";
    }
}

// This is a naive algorithm for 'hashing' the images in a way that we can order them by by their distance from one
// another. We can imagine that a better algorithm will be plugged in here.
uint64_t hash(const cv::Mat& mat)
{
    uint64_t sum = 0;
    const uint64_t sz = (uint64_t)mat.rows * (uint64_t)mat.cols;
//    cv::Mat adapted;
//    cv::adaptiveThreshold(mat, adapted, 190, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::ADAPTIVE_THRESH_MEAN_C, 11, 0);

    for (int r = 0; r < mat.rows; r++)
    {
        // Loop over all columns
        for ( int c = 0; c < mat.cols; c++)
        {
            const auto pixel = mat.at<uchar>(r, c);
            sum += pixel;
        }
    }

    return sum;
}
