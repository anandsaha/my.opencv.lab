#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

static const int MAX_CORNERS = 1000;

int main()
{

    // Read images
    Mat img1 = imread("images/example2/1.png", cv::IMREAD_GRAYSCALE);
    Mat img2 = imread("images/example2/2.png", cv::IMREAD_GRAYSCALE);
    Mat img2a= imread("images/example2/2.png", cv::IMREAD_UNCHANGED);
    cv::Size img1_size = img1.size();
    int win_size = 10;

    // Get features you want to track
    vector<cv::Point2f> corners1, corners2;

    // Extract good features to track in first image
    cv::goodFeaturesToTrack(img1, corners1, MAX_CORNERS, 0.01, 5, cv::noArray(), 3, false, 0.04);

    // Refine them
    cv::cornerSubPix(img1, corners1, cv::Size(win_size, win_size), cv::Size(-1, -1), 
            cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 20, 0.03));

    // Find the features in the second image
    vector<uchar> features_found;
    cv::calcOpticalFlowPyrLK(img1, img2, corners1, corners2, features_found, cv::noArray(),
            cv::Size(win_size*2+1, win_size*2+1), 5, cv::TermCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 20, 0.3));


    // Draw lines showing the features that have moved
    for( int i = 0; i < (int)corners1.size(); i++ ) {
        if( !features_found[i] )
            continue;

        line(img2a, corners1[i], corners2[i], Scalar(0,255,0), 2, cv::LINE_AA);
    }

    //cv::imshow( "ImageA", img1 );
    //cv::imshow( "ImageB", img2 );
    //cv::imshow( "LK Optical Flow Example", img2a );
    //cv::waitKey(0);
    cv::imwrite("images/example2/lk_optical.png", img2a);

    return 0;
}
