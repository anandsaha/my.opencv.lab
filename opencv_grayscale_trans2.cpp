#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;



Mat getNegativeTx() {
    Mat LUT(1, 256, CV_8U);

    for(int i = 0; i < 256; ++i)
        LUT.at<uchar>(0, i) = uchar(255 - i);

    return LUT;
}

// Negative Transformation using OpenCV's LUT function
int main()
{
    Mat image = imread("images/Lenna.jpg", IMREAD_GRAYSCALE);
    LUT(image, getNegativeTx(), image);
    imshow("Negative transformation", image);
    waitKey(0);
    return 0;
}
