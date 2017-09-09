#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <vector>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include "opencv2/bgsegm.hpp"

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    Mat image = imread(argv[1], cv::IMREAD_UNCHANGED);
    Mat result;

    imshow("Original", image);
    waitKey(0);

    cv::copyMakeBorder(image.clone(), result, 10, 10, 10, 10, cv::BORDER_CONSTANT, cv::Scalar(100, 100, 0));
    imshow("copyMakeBorder", result);
    waitKey(0);

    cv::threshold(image.clone(), result, 100, 100, cv::THRESH_TOZERO);
    imshow("threshold", result);
    waitKey(0);

    // Splitting image into 3 channels and combining them back
    vector<Mat> channels;
    cv::split(image.clone(), channels);
    Mat merged = cv::Mat::zeros(channels[0].size(), CV_32F);
    //cv::accumulate(channels[0], merged);
    //cv::accumulate(channels[1], merged);
    //cv::threshold(merged, merged, 255, 255, cv::THRESH_TRUNC);
    //merged.convertTo(merged, channels[0].type());
    cv::merge(channels, merged);
    imshow("After merging", merged);
    waitKey(0);

    //cvtColor(image, gray, cv::COLOR_BGR2GRAY);


    return 0;
}
