#include <iostream>
#include <vector>
#include <set>

#include "opencv2/opencv.hpp"
#include "Slice.h"


uint64_t hash(const cv::Mat& mat);

int main()
{
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
        std::cout << h << std::endl;
    }

    const auto samples = sample(slices, 4);
    std::cout << "hello world" << std::endl;
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
