#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void imageTraversal(Mat image) {
	
    Mat_<Vec3d> _I = image;

    for(int r = 0; r < _I.rows; ++r) {
        for(int c = 0; c < _I.cols; ++c) {

            //cout << int(_I(r, c)[0]) << ", " << int(_I(r, c)[1]) << ", " << int(_I(r, c)[2]) << endl;
            _I(r, c)[0] = 255 - int(_I(r, c)[0]);
            _I(r, c)[1] = 255 - int(_I(r, c)[1]);
            _I(r, c)[2] = 255 - int(_I(r, c)[2]);

        }
    }

    imshow("Negative", _I);
    waitKey(0);
    cv::destroyAllWindows();
}

int main()
{
	Mat image = imread("/home/anand/work/Lenna.jpg", IMREAD_COLOR);
    imageTraversal(image.clone());
    waitKey(0);

    return 0;
}
