#include <iostream>


#include "opencv2/opencv.hpp"


//int main()
//{
//    cv::VideoCapture cap(0); // open the default camera
//    if(!cap.isOpened())  // check if we succeeded
//        return -1;
//
//    cv::Mat edges;
//    cv::namedWindow("edges",1);
//    for(;;)
//    {
//        cv::Mat frame;
//        cap >> frame; // get a new frame from camera
//        cvtColor(frame, edges, cv::COLOR_BGR2GRAY);
//        GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
//        Canny(edges, edges, 0, 30, 3);
//        imshow("edges", edges);
//        if(cv::waitKey(30) >= 0) break;
//    }
//    // the camera will be deinitialized automatically in VideoCapture destructor
//    return 0;
//}

uint64_t hash(const cv::Mat& mat);

int main()
{
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

        std::cout << h << std::endl;
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
