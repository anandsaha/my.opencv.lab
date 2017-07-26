#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{

    Mat input_image = imread("images/Lenna.jpg", IMREAD_GRAYSCALE);
    Mat output_image;


    imshow("Filtered Image", output_image);
    waitKey(0);

    return 0;
}
