#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#define KSIZE 7 

int main()
{

    Mat input_image = imread("images/Lenna.jpg", IMREAD_GRAYSCALE);
    Mat output_image;

    Mat kernel = Mat::ones(KSIZE, KSIZE, CV_32F) / float(KSIZE * KSIZE);
    filter2D(input_image, output_image, -1, kernel, Point(-1, -1), 0, BORDER_REPLICATE);

    imshow("Filtered Image", output_image);
    waitKey(0);

    return 0;
}
