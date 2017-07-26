#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("/home/anand/work/git/my.opencv.lab/images/Lenna.jpg", IMREAD_COLOR);
    Mat output;

    boxFilter(image,
            output,
            -1,
            Size(3,3),
            Point(-1, -1),
            true,
            BORDER_REPLICATE);

    imshow("Original", image);
    imshow("Transformed", output);
    waitKey(0);

    return 0;
}
