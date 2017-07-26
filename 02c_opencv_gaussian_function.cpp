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

    GaussianBlur(input_image, output_image, Size(7, 7), 2.0, 2.0, BORDER_REPLICATE);

    imshow("Filtered Image", output_image);
    waitKey(0);

    return 0;
}
