#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace cv;

void imageTraversal(Mat image) {

    Mat_<Vec3d> _I = image;

    for(int r = 0; r < _I.rows; ++r) {
        for(int c = 0; c < _I.cols; ++c) {

            //cout << int(_I(r, c)[0]) << ", " << int(_I(r, c)[1]) << ", " << int(_I(r, c)[2]) << endl;
            //_I(r, c)[0] = 255 - int(_I(r, c)[0]);
            //_I(r, c)[1] = 255 - int(_I(r, c)[1]);
            //_I(r, c)[2] = 255 - int(_I(r, c)[2]);

        }
    }

    imshow("Negative", _I);
    waitKey(0);
    cv::destroyAllWindows();
}

int effective(int after, int before) {
    return after;
    int a = abs(before - after);
    if(a < 50)
        return after;
    else
        return after - before;
}

int main()
{
    Mat image1 = imread("images/DSC_0138.png", IMREAD_COLOR);
    Mat image2 = imread("images/DSC_0139.png", IMREAD_COLOR);
    Mat diff(Size(image1.cols, image1.rows), CV_8UC3, Scalar(255, 255, 255)); 

    int rows = image1.rows;
    int cols = image1.cols * image1.channels();
    for(int i = 0; i < rows; i++) {
        uchar* img1_row = image1.ptr<uchar>(i);
        uchar* img2_row = image2.ptr<uchar>(i);
        uchar* diff_row = diff.ptr<uchar>(i);
        for (int j = 0; j < cols; j++)
            diff_row[j] = img_row2[j];
    }


/*
    for(int r = 0; r < image1.rows; ++r) {
        for(int c = 0; c < image1.cols; ++c) {
            diff.at<Vec3d>(r, c)[0] = int(image2.at<Vec3d>(r, c)[0]);
            diff.at<Vec3d>(r, c)[0] = int(image2.at<Vec3d>(r, c)[0]);
            diff.at<Vec3d>(r, c)[0] = int(image2.at<Vec3d>(r, c)[0]);
            //d(r, c)[0] = effective(int(i2(r, c)[0]), int(i1(r, c)[0])); 
            //d(r, c)[1] = effective(int(i2(r, c)[1]), int(i1(r, c)[1])); 
            //d(r, c)[2] = effective(int(i2(r, c)[2]), int(i1(r, c)[2])); 
        }
    }
*/

    imshow("Diff", diff);
    waitKey(0);

    return 0;
}
